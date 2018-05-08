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
#include "parse_args.h"
// #include <aubio.h>
#include <spectral/statistics.c>
#include <spectral/specdesc.h>

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
fvec_t *centroid;
fvec_t *spread;
fvec_t *skewness;
fvec_t *kurtosis;
fvec_t *decrease;
fvec_t *rolloff;

float sample;
// fvec_t *spectral_slope;
uint_t n_filters = 40;
uint_t n_coefs = 13;

void process_block (fvec_t *ibuf, fvec_t *obuf)
{
  fvec_zeros(obuf);

  //compute mag spectrum
  aubio_pvoc_do (pv, ibuf, fftgrain);

  //compute mfccs
  aubio_mfcc_do(mfcc, fftgrain, mfcc_out);

  // calc spectral slope
  slope = new_fvec (1); 
  aubio_specdesc_do (spectre_slope, fftgrain, slope);

  // calc spectral centroid
  centroid = new_fvec (1); 
  aubio_specdesc_do (spectre_centroid, fftgrain, centroid);

  // calc spectral spread
  spread = new_fvec (1);
  aubio_specdesc_do (spectre_spread, fftgrain, spread);

  // calc spectral skewness
  skewness = new_fvec (1);
  aubio_specdesc_do (spectre_skewness, fftgrain, skewness);

  // calc spectral kurtosis
  kurtosis = new_fvec (1);
  aubio_specdesc_do (spectre_kurtosis, fftgrain, kurtosis);

  // calc spectral decrease
  decrease = new_fvec (1);
  aubio_specdesc_do (spectre_decrease, fftgrain, decrease);

  // calc spectral rolloff
  rolloff = new_fvec (1);
  aubio_specdesc_do (spectre_rolloff, fftgrain, rolloff);

}

void process_print (void)
{
  /* output times in selected format */
  print_time (blocks * hop_size);
  // outmsg ("\t");

  

  // outmsg("");
  // actually a vector mean (loudness)
  printf("%9f", cvec_mean (fftgrain) );
  printf("%11f", cvec_centroid (fftgrain) );
  printf("%10f", slope->data[0] );
  printf("%11f", centroid->data[0] );
  printf("%14f", spread->data[0] );
  printf("%10f", skewness->data[0] );
  printf("%10f", kurtosis->data[0] );
  printf("%10f", decrease->data[0] );
  printf("%11f", rolloff->data[0] );
  

  // fvec_print(slope);
  // fvec_print(centroid);
  // fvec_print(spread);

  // printf("fftgrain length: %i   ", fftgrain->length);
  // sample = cvec_norm_get_sample(fftgrain,0); 

  // printf("spectral slope: %f\n", sample);	


  // fvec_print (fftgrain->norm);
  /* output fftgrain as is */
  // cvec_print (fftgrain);


  /* output extracted mfcc */	
  // fvec_print (mfcc_out);
  outmsg("\n");

}

int main(int argc, char **argv) {
  int ret = 0;
  // change some default params
  buffer_size  = 512;
  // hop_size = 256;
  hop_size = 256;
  // window_size = 1024;



  examples_common_init(argc,argv);

  verbmsg ("using source: %s at %dHz\n", source_uri, samplerate);
  verbmsg ("buffer_size: %d, ", buffer_size);
  verbmsg ("hop_size: %d\n", hop_size);

  pv = new_aubio_pvoc (buffer_size, hop_size);
  fftgrain = new_cvec (buffer_size);

  spectre_slope = new_aubio_specdesc ("slope", buffer_size);
  spectre_centroid = new_aubio_specdesc ("centroid", buffer_size);
  spectre_spread = new_aubio_specdesc ("spread", buffer_size);
  spectre_skewness = new_aubio_specdesc ("skewness", buffer_size);
  spectre_kurtosis = new_aubio_specdesc ("kurtosis", buffer_size);
  spectre_decrease = new_aubio_specdesc ("decrease", buffer_size);
  spectre_rolloff  = new_aubio_specdesc ("rolloff", buffer_size);
  
  printf("%-9s","time");
  printf("%-9s","mean");
  printf("%-11s","centroid1");
  printf("%-10s","slope");
  printf("%-12s","centroid2");
  printf("%-14s","spread");
  printf("%-9s","skewness");
  printf("%-11s","kurtosis");
  printf("%-9s","decrease");
  printf("%-11s","rolloff");
  // printf("%s","mfcc");
  outmsg( "\n" );

  mfcc = new_aubio_mfcc(buffer_size, n_filters, n_coefs, samplerate);
  mfcc_out = new_fvec(n_coefs);
  if (pv == NULL || fftgrain == NULL) {
    ret = 1;
    goto beach;
  }

  // process_block()
  // process_print()	

  examples_common_process((aubio_process_func_t)process_block, process_print);

  del_aubio_pvoc (pv);
  del_aubio_specdesc (spectre_slope);
  del_aubio_specdesc (spectre_centroid);
  del_aubio_specdesc (spectre_spread);
  del_aubio_specdesc (spectre_skewness);
  del_aubio_specdesc (spectre_kurtosis);
  del_aubio_specdesc (spectre_decrease);
  del_aubio_specdesc (spectre_rolloff);

  del_cvec (fftgrain);
  // del_aubio_mfcc(mfcc);
  // del_fvec(mfcc_out);

beach:
  examples_common_del();
  return ret;
}
