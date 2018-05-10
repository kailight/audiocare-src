/**
 * @file jacktest.c
 *
 * @brief Simple client to test JACK on RPI
 */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <jack/jack.h>

typedef jack_default_sample_t jack_sample_t;

jack_port_t *input_port;
jack_port_t *output_port;
jack_client_t *client;
jack_sample_t ibufs[5];
int crun = 0;

FILE *f;


void
done() {
  jack_client_close(client);
  for (i = 0; i < 5; i++) {
    fprintf(f, "%i\n", i);
  }
  exit(1);
}

/**
 * The process callback for this JACK application is called in a
 * special realtime thread once for each audio cycle.
 *
 * This client does nothing more than copy data from its input
 * port to its output port. It will exit when stopped by
 * the user (e.g. using Ctrl-C on a unix-ish operating system)
 */
int
process (jack_nframes_t nframes, void *arg)
{
    jack_default_audio_sample_t *in, *out;

    ibufs[crun] = (jack_sample_t *) jack_port_get_buffer (input_port, nframes);
    crun++;

    if (crun > 5) {
      done();
    }

    // printf("yay");
    // return 1;

    in = jack_port_get_buffer (input_port, nframes);

    /* print some text */
    // printf("Jack data");
    // fprintf(f, "Jack data\n");

    /* print integers and floats */
    // fprintf(f, "data: %f\n", in[0]);

    out = jack_port_get_buffer (output_port, nframes);
    memcpy (out, in, sizeof (jack_default_audio_sample_t) * nframes);

    return 0;
}




/**
 * JACK calls this shutdown_callback if the server ever shuts down or
 * decides to disconnect the client.
 */
void
jack_shutdown (void *arg)
{
    printf("Exiting\n");
    fprintf(f, "Exiting\n");
    exit (1);
}

void
jack_thread_init(void *arg) {
  printf("thread init\n");
}


// Called when ctrl-c is pressed
void
signalHandler(int sig)
{
    printf("CTRL+C'ed\n");
    fprintf(f, "CTRL+C'ed\n");
}


int
main (int argc, char *argv[])
{
    const char **ports;
    const char *client_name = "simple";
    const char *server_name = NULL;
    jack_options_t options = JackNullOption;
    jack_status_t status;

    /* write something to file data.txt */
    f = fopen("data.txt", "a");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    printf("test\n");
    fprintf(f, "Starting\n");

    /* open a client connection to the JACK server */
    client = jack_client_open (client_name, options, &status, server_name);
    if (client == NULL) {
        fprintf (stderr, "jack_client_open() failed, "
                         "status = 0x%2.0x\n", status);
        if (status & JackServerFailed) {
            fprintf (stderr, "Unable to connect to JACK server\n");
        }
        exit (1);
    }
    if (status & JackServerStarted) {
        fprintf (stderr, "JACK server started\n");
    }
    if (status & JackNameNotUnique) {
        client_name = jack_get_client_name(client);
        fprintf (stderr, "unique name `%s' assigned\n", client_name);
    }


    jack_set_thread_init_callback( client, jack_thread_init, 0 );
    printf("Thread callback set\n");

    /* tell the JACK server to call `process()' whenever
       there is work to be done.
    */
    jack_set_process_callback (client, process, 0);

    printf("Process callback set\n");

    /* tell the JACK server to call `jack_shutdown()' if
       it ever shuts down, either entirely, or if it
       just decides to stop calling us.
    */
    jack_on_shutdown (client, jack_shutdown, 0);

    /* display the current sample rate.
     */
    printf ("Engine sample rate: %" PRIu32 "\n",
            jack_get_sample_rate (client));

    fprintf(f, "Test exit\n");
    exit(1);

    /* create two ports */

    input_port = jack_port_register (client, "input",
                                     JACK_DEFAULT_AUDIO_TYPE,
                                     JackPortIsInput, 0);
    output_port = jack_port_register (client, "output",
                                      JACK_DEFAULT_AUDIO_TYPE,
                                      JackPortIsOutput, 0);

    if ( (input_port == NULL) || (output_port == NULL) ) {
        fprintf(stderr, "no more JACK ports available\n");
        exit (1);
    }

    /* Tell the JACK server that we are ready to roll.  Our
     * process() callback will start running now. */

    if (jack_activate (client)) {
        fprintf (stderr, "cannot activate client");
        exit (1);
    }


    // The next part should be optional - it's alway a bad thing to connect ports automatically.

    /* Connect the ports.  You can't do this before the client is
     * activated, because we can't make connections to clients
     * that aren't running.  Note the confusing (but necessary)
     * orientation of the driver backend ports: playback ports are
     * "input" to the backend, and capture ports are "output" from
     * it.
     */

    ports = jack_get_ports (client, NULL, NULL,
                            JackPortIsPhysical|JackPortIsOutput);

    if (ports == NULL) {
        fprintf(stderr, "no physical capture ports\n");
        exit (1);
    }

    if (jack_connect (client, ports[0], jack_port_name (input_port))) {
        fprintf (stderr, "cannot connect input ports\n");
    }

    free (ports);

    ports = jack_get_ports (client, NULL, NULL,
                            JackPortIsPhysical|JackPortIsInput);
    if (ports == NULL) {
        fprintf(stderr, "no physical playback ports\n");
        exit (1);
    }

    if (jack_connect (client, jack_port_name (output_port), ports[0])) {
        fprintf (stderr, "cannot connect output ports\n");
    }

    free (ports);

    // If you kill this program you'll see xruns for sure. Better call jack_client_close( client ) via atexit(..).

    /* keep running until stopped by the user */

    sleep (-1);

    /* this is never reached but if the program
       had some other way to exit besides being killed,
       they would be important to call.
    */

    jack_client_close (client);
    exit (0);
}


