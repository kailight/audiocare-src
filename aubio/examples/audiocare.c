/*
  Copyright (C) 2007-2013 Paul Brossier <piem@aubio.org>

  This file is part of aubio.

  aubio is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  aubio is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with aubio.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "utils.h"
#include "jackio.h"
#define PROG_HAS_MULTIDATA 1
#define PROG_HAS_JACK 1
#define PROG_HAS_OUTPUT 1
#include "parse_args.h"
// #include <aubio.h>
#include <spectral/statistics.c>
#include <spectral/specdesc.h>
#include <time.h>
#include <stdbool.h>

aubio_wavetable_t *wavetable;
aubio_pvoc_t *pv;    // a phase vocoder
cvec_t *fftgrain;    // outputs a spectrum
aubio_mfcc_t * mfcc; // which the mfcc will process
fvec_t * mfcc_out;   // to get the output coefficients
aubio_specdesc_t *spectre_slope;
aubio_specdesc_t *spectre_centroid;
aubio_specdesc_t *spectre_spread;
aubio_specdesc_t *spectre_skewness;
aubio_specdesc_t *spectre_kurtosis;
aubio_specdesc_t *spectre_decrease;
aubio_specdesc_t *spectre_rolloff;


fvec_t *spectre_vector;
// fvec_t *out;
// smpl_t *slope;
fvec_t *slope;
smpl_t *centroid;
fvec_t *spread;
fvec_t *skewness;
fvec_t *kurtosis;
fvec_t *decrease;
fvec_t *rolloff;

bool flag_mean;
bool flag_centroid;
bool flag_slope;
bool flag_spread;
bool flag_skewness;
bool flag_kurtosis;
bool flag_decrease;
bool flag_rolloff;
bool flag_mfcc;

float sample;
// fvec_t *spectral_slope;
uint_t n_filters = 40;
uint_t n_coefs = 13;

aubio_pitch_t *o;
fvec_t *pitch;

void process_block (fvec_t *ibuf, fvec_t *obuf, aubio_jack_t * jack_setup)
{
  fvec_zeros(obuf);

  fvec_print(jack_setup->ibuf);

  return;

  // add something to obuf
  // aubio_onset_do
  // aubio_pitch_do (o, ibuf, pitch);
  if ( !usejack && !sink_uri ) {
    outmsg("NO JACK\n");
    return;
  }
  // aubio_wavetable_set_amp ( wavetable, aubio_level_lin (ibuf) );
  // aubio_wavetable_set_freq ( wavetable, freq );
  aubio_wavetable_do (wavetable, ibuf, obuf);

  // fvec_print(ibuf);
  // fvec_print(obuf);

  // outmsg("processing_block\n");
  return;

  //compute mag spectrum
  aubio_pvoc_do (pv, ibuf, fftgrain);

  //compute mfccs
  if (flag_mfcc) {
    aubio_mfcc_do(mfcc, fftgrain, mfcc_out);
  }

  // calc spectral slope
  if (flag_slope) {
  	slope = new_fvec (1);
	aubio_specdesc_do (spectre_slope, fftgrain, slope);
  }

  // calc spectral centroid
  // if (flag_centroid) {
    // centroid = new_fvec (1);
    // centroid = cvec_centroid (fftgrain);
  	// aubio_specdesc_do (spectre_centroid, fftgrain, centroid);
  // }

  // calc spectral spread
  if (flag_spread) {
  	spread = new_fvec (1);
  	aubio_specdesc_do (spectre_spread, fftgrain, spread);
  }

  // calc spectral skewness
  if (flag_skewness) {
  	skewness = new_fvec (1);
  	aubio_specdesc_do (spectre_skewness, fftgrain, skewness);
  }

  // calc spectral kurtosis
  if (flag_kurtosis) {
    kurtosis = new_fvec (1);
    aubio_specdesc_do (spectre_kurtosis, fftgrain, kurtosis);
  }

  // calc spectral decrease
  if (flag_decrease) {
    decrease = new_fvec (1);
    aubio_specdesc_do (spectre_decrease, fftgrain, decrease);
  }

  // calc spectral rolloff
  if (flag_rolloff) {
    rolloff = new_fvec (1);
    aubio_specdesc_do (spectre_rolloff, fftgrain, rolloff);
  }

  process_print();

}

void process_print (void)
{

  outmsg("processing_print\n");
  print_time(blocks * hop_size);

  /* output times in selected format */
  // print_time (blocks * hop_size);
  // outmsg ("\t");

  time_t current_time;
  // char times;
  // Obtain current time.
  current_time = time(NULL);
  // Convert to local time format.
  // times = ctime(&current_time);

  // outmsg("====================================================");

  // actually a vector mean (loudness)
  printf("%li ", current_time );
  if (flag_mean)     printf("%f ",  cvec_mean (fftgrain) );
  if (flag_centroid) printf("%f ",  cvec_centroid (fftgrain) );
  if (flag_slope)    printf("%f ",  slope->data[0] );
  //printf("%11f", centroid->data[0] );
  if (flag_spread)   printf("%f ",  spread->data[0] );
  if (flag_skewness) printf("%f ",  skewness->data[0] );
  if (flag_kurtosis) printf("%f ",  kurtosis->data[0] );
  if (flag_decrease) printf("%f ",  kurtosis->data[0] );
  if (flag_rolloff)  printf("%f ",  rolloff->data[0] );

  // printf("fftgrain length: %i   ", fftgrain->length);
  // sample = cvec_norm_get_sample(fftgrain,0);
  // printf("spectral slope: %f\n", sample);


  // fvec_print (fftgrain->norm);
  /* output fftgrain as is */
  // cvec_print (fftgrain);


  /* output extracted mfcc */
  if (flag_mfcc) {
    outmsg("|");
    fvec_print (mfcc_out);
  } else {
    outmsg("\n");
  }

}

int main(int argc, char **argv) {
  int ret = 0;
  // change some default params
  buffer_size  = 2048;
  // hop_size = 256;
  hop_size = 1024;
  // window_size = 1024;

  examples_common_init(argc,argv);

  // printf("datas: %s\n", datas);
  // printf("typeof datas - %s", type(datas));
  // 48 is 0, 49 is 1
  // printf("datas[0] - %i\n", datas[0]);
  // printf("datas[1] - %i", datas[1]);
  if (datas[0] == 49) flag_mean = true;     else flag_mean = false;
  if (datas[1] == 49) flag_centroid = true; else flag_centroid = false;
  if (datas[2] == 49) flag_slope = true;    else flag_slope = false;
  if (datas[3] == 49) flag_spread = true;   else flag_spread = false;
  if (datas[4] == 49) flag_skewness = true; else flag_skewness = false;
  if (datas[5] == 49) flag_kurtosis = true; else flag_kurtosis = false;
  if (datas[6] == 49) flag_decrease = true; else flag_decrease = false;
  if (datas[7] == 49) flag_rolloff = true;  else flag_rolloff = false;
  if (datas[8] == 49) flag_mfcc = true;     else flag_mfcc = false;



						printf("==time ");
  if (flag_mean) 		printf("mean ");
  if (flag_centroid) 	printf("centroid ");
  if (flag_slope) 	    printf("slope ");
  if (flag_spread) 	    printf("spread ");
  if (flag_skewness)    printf("skewness ");
  if (flag_kurtosis)    printf("kurtosis ");
  if (flag_decrease)    printf("decrease ");
  if (flag_rolloff)     printf("rolloff ");
  if (flag_mfcc)        printf("mfcc ");
						printf("==\n");



  verbmsg ("using source: %s at %dHz\n", source_uri, samplerate);
  verbmsg ("buffer_size: %d, ", buffer_size);
  verbmsg ("hop_size: %d\n", hop_size);
  verbmsg("DATA: mean:%d centroid:%d slope:%d spread:%d skewness:%d kurtosis:%d decrease:%d rolloff:%d mfcc:%d\n", flag_mean, flag_centroid, flag_slope, flag_spread, flag_skewness, flag_kurtosis, flag_decrease, flag_rolloff, flag_mfcc );

  pv = new_aubio_pvoc (buffer_size, hop_size);
  fftgrain = new_cvec (buffer_size);

  // spectre_centroid = new_aubio_specdesc ("centroid", buffer_size);
  if (flag_slope)	
    spectre_slope = new_aubio_specdesc ("slope", buffer_size);
  if (flag_spread)
    spectre_spread = new_aubio_specdesc ("spread", buffer_size);	
  if (flag_skewness)
    spectre_skewness = new_aubio_specdesc ("skewness", buffer_size);	
  if (flag_kurtosis)
    spectre_kurtosis = new_aubio_specdesc ("kurtosis", buffer_size);
  if (flag_decrease)
    spectre_decrease = new_aubio_specdesc ("decrease", buffer_size);
  if (flag_rolloff)
    spectre_rolloff  = new_aubio_specdesc ("rolloff", buffer_size);
  if (flag_mfcc) {   
    mfcc = new_aubio_mfcc(buffer_size, n_filters, n_coefs, samplerate);
    mfcc_out = new_fvec(n_coefs);
  }

  if (pv == NULL || fftgrain == NULL) {
    ret = 1;
    goto beach;
  }

  // process_block()
  // process_print()	

  // wavetable = new_aubio_wavetable (samplerate, hop_size);
  // aubio_wavetable_play ( wavetable );

  examples_common_process((aubio_process_func_t)process_block, process_print);

  // del_aubio_specdesc (spectre_centroid);
  del_aubio_pvoc (pv);
  del_cvec (fftgrain);
  if (flag_slope)    del_aubio_specdesc (spectre_slope);
  if (flag_spread)   del_aubio_specdesc (spectre_spread);
  if (flag_skewness) del_aubio_specdesc (spectre_skewness);
  if (flag_kurtosis) del_aubio_specdesc (spectre_kurtosis);
  if (flag_decrease) del_aubio_specdesc (spectre_decrease);
  if (flag_rolloff)  del_aubio_specdesc (spectre_rolloff);
  if (flag_mfcc) {
    del_aubio_mfcc(mfcc);
    del_fvec(mfcc_out);
  }
  del_aubio_wavetable(wavetable);

beach:
  examples_common_del();
  return ret;
}
