/*
  Copyright (C) 2003-2013 Paul Brossier <piem@aubio.org>

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

#ifndef JACKIO_H
#define JACKIO_H

/**
 * @file
 *
 * Jack driver for aubio
 *
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include <jack/jack.h>
#include <jack/midiport.h>
#include <jack/ringbuffer.h>

/** jack object */
typedef struct _aubio_jack_t aubio_jack_t;

/** jack device creation function */
aubio_jack_t *new_aubio_jack (uint_t hop_size,
    uint_t inchannels, uint_t outchannels,
    uint_t imidichan, uint_t omidichan);
/** activate jack client (run jackprocess function) */
uint_t aubio_jack_activate (aubio_jack_t * jack_setup,
    aubio_process_func_t callback);
/** close and delete jack client */
void aubio_jack_close (aubio_jack_t * jack_setup);
void del_aubio_jack (aubio_jack_t * jack_setup);
/** get samplerate */
uint_t aubio_jack_get_samplerate (aubio_jack_t * jack_setup);

/** write a jack_midi_event_t to the midi output ringbuffer */
void aubio_jack_midi_event_write (aubio_jack_t * jack_setup,
    jack_midi_event_t * event);


/**
 * jack device structure
 */
struct _aubio_jack_t
{
    /** jack client */
    jack_client_t *client;
    /** jack output ports */
    jack_port_t **oports;
    /** jack input ports */
    jack_port_t **iports;
    /** jack input buffer */
    jack_sample_t **ibufs;
    /** jack output buffer */
    jack_sample_t **obufs;
#ifdef AUBIO_JACK_NEEDS_CONVERSION
    /** converted jack input buffer */
  smpl_t **sibufs;
  /** converted jack output buffer */
  smpl_t **sobufs;
#endif
    /** jack input audio channels */
    uint_t ichan;
    /** jack output audio channels */
    uint_t ochan;
    /** jack input midi channels */
    uint_t imidichan;
    /** jack output midi channels */
    uint_t omidichan;
    /** midi output ringbuffer */
    jack_ringbuffer_t *midi_out_ring;
    /** jack samplerate (Hz) */
    uint_t samplerate;
    /** jack processing function */
    aubio_process_func_t callback;
    /** internal fvec */
    fvec_t *ibuf;
    fvec_t *obuf;
    uint_t hop_size;
    int pos;
};




#ifdef __cplusplus
}
#endif

#endif                          /* JACKIO_H */
