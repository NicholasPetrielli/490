#include <pocketsphinx.h>
#include <time.h>
#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

static ps_decoder_t *ps;
static cmd_ln_t *config;
static FILE *rawfd;

static const arg_t cont_args_def[] = {
    POCKETSPHINX_OPTIONS,
    /* Argument file. */
    {"-argfile",
     ARG_STRING,
     NULL,
     "Argument file giving extra arguments."},
    {"-adcdev",
     ARG_STRING,
     NULL,
     "Name of audio device to use for input."},
    {"-infile",
     ARG_STRING,
     NULL,
     "Audio file to transcribe."},
    {"-inmic",
     ARG_BOOLEAN,
     "yes",
     "Transcribe audio from microphone."},
    {"-time",
     ARG_BOOLEAN,
     "no",
     "Print word times in file transcription."},
    CMDLN_EMPTY_OPTION
};

/* Sleep for specified msec */
static void
sleep_msec(int32 ms)
{
#if (defined(_WIN32) && !defined(GNUWINCE)) || defined(_WIN32_WCE)
    Sleep(ms);
#else
    /* ------------------- Unix ------------------ */
    struct timeval tmo;

    tmo.tv_sec = 0;
    tmo.tv_usec = ms * 1000;

    select(0, NULL, NULL, NULL, &tmo);
#endif
}

void getWeather(){
        system("curl \"http://api.openweathermap.org/data/2.5/weather?q=Toronto&APPID=279adcf563de661f5c8b7dd9dee187c5\" > file.txt");
        system("tr ',' '\n' < file.txt >file2.txt");
        system("tr -d '\"' < file2.txt > file.txt");
        system("tr -d '[' < file.txt > file2.txt");
        system("tr -d ']' < file2.txt > file.txt");
        system("tr -d '{' < file.txt > file2.txt");
        system("tr -d '}' < file2.txt > file.txt");
        system("tr ':' ' ' < file.txt > file2.txt");
        system("rm file.txt");
        FILE *fp;
        char buff[255];
        char buffCopy[255];
        char commandBuffer[600];
        system("> weather.txt");

        fp = fopen("file2.txt", "r");

        while(fgets(buff, 255, (FILE*)fp)){
                strcpy(buffCopy,buff);
                int i = 0;
                while( i < 255){
                if(buffCopy[i] == '\n'){
                        buffCopy[i] ='\0';
                }
                i++;
                }
                char* token = strtok(buff, " ");
                while(token){
                if        (strcmp(token, "description") == 0){
                        sprintf(commandBuffer, "echo %s >> weather.txt", buffCopy);
                        system(commandBuffer);
                } else if (strcmp(token, "humidity") == 0){
                        sprintf(commandBuffer, "echo %s >> weather.txt", buffCopy);
                        system(commandBuffer);
                } else if (strcmp(token, "main") == 0){
                        token = strtok(NULL, " ");
                        if (strcmp(token, "temp") == 0){
                                token = strtok(NULL, " ");
                                float num = atof(token) - 273.15;
                                sprintf(commandBuffer, "echo The temperature is %.1f degrees celsius >> weather.txt", num);
                                system(commandBuffer);
                        }
                }
                token = strtok(NULL, " ");
                }
        }
        fclose(fp);
	system("rm file2.txt");
}

void getMemo(){
	FILE *fp = fopen("memo.txt","r");
	if (fp == NULL){
		//do nothing
	} else {
		system("espeak 'You left the following memo' -s 135 2>/dev/null");
		system("espeak -f memo.txt -s 135 2>/dev/null");
		system("rm memo.txt");
	}
fclose(fp);

}

char const* play_goforward()
{
	ps_decoder_t *ps;
    cmd_ln_t *config;
    FILE *fh;
    char const *hyp, *uttid;
    int16 buf[512];
    int rv;
    int32 score;
    char *temp;
    config = cmd_ln_init(NULL, ps_args(), TRUE,
                 "-hmm", MODELDIR "/en-us/en-us",
                 "-lm", MODELDIR "/en-us/en-us.lm.bin",
                 "-dict", MODELDIR "/en-us/cmudict-en-us.dict",
                 NULL);
    if (config == NULL) {
        fprintf(stderr, "Failed to create config object, see log for details\n");
        return;
    }
    
    ps = ps_init(config);
    if (ps == NULL) {
        fprintf(stderr, "Failed to create recognizer, see log for details\n");
        return;
    }

    fh = fopen("goforward.raw", "rb");
    if (fh == NULL) {
        fprintf(stderr, "Unable to open input file goforward.raw\n");
        return;
    }

    rv = ps_start_utt(ps);
    
    while (!feof(fh)) {
        size_t nsamp;
        nsamp = fread(buf, 2, 512, fh);
        rv = ps_process_raw(ps, buf, nsamp, FALSE, FALSE);
    }
    
    rv = ps_end_utt(ps);
    hyp = ps_get_hyp(ps, &score);
    printf("Recognized: %s\n", hyp);
    fclose(fh);
    return hyp;
    ps_free(ps);
    cmd_ln_free_r(config);
    printf("still: %s\n",temp);
    return hyp;
}

char const* try_to_record(){
	ps_decoder_t *ps;
    cmd_ln_t *config;
    FILE *fh;
    char const *hyp, *uttid;
    int16 buf[512];
    int rv;
    int32 score;
    int16* bufWrite[512];
    int32* sizeWrite;
    config = cmd_ln_init(NULL, ps_args(), TRUE,
                 "-hmm", MODELDIR "/en-us/en-us",
                 "-lm", MODELDIR "/en-us/en-us.lm.bin",
                 "-dict", MODELDIR "/en-us/cmudict-en-us.dict",
                 NULL);
    if (config == NULL) {
        fprintf(stderr, "Failed to create config object, see log for details\n");
        return;
    }
    
    ps = ps_init(config);
    if (ps == NULL) {
        fprintf(stderr, "Failed to create recognizer, see log for details\n");
        return;
    }

    
    clock_t start = clock();
    clock_t endTime = clock();
    printf("say something now \n");
    rv = ps_start_utt(ps);
    ps_get_rawdata(ps,bufWrite,sizeWrite);
    while((double)(endTime-start)/CLOCKS_PER_SEC < 5){
    endTime = clock();
    }
    rv = ps_end_utt(ps);
    
    fh = fopen("output.raw", "w");
    int i = 0;
    while(i < 500){
    fputc(buf[i],fh);
    i++;
    }
    fclose(fh);
    fh = fopen("output.raw", "rb");
    if (fh == NULL) {
        fprintf(stderr, "Unable to open input file goforward.raw\n");
        return;
    }

    rv = ps_start_utt(ps);
    
    while (!feof(fh)) {
        size_t nsamp;
        nsamp = fread(buf, 2, 512, fh);
	rv = ps_process_raw(ps, buf, nsamp, FALSE, FALSE);
    }
    
    rv = ps_end_utt(ps);
    hyp = ps_get_hyp(ps, &score);
    printf("Recognized: %s\n", hyp);
    fclose(fh);
    return hyp;
    ps_free(ps);
    cmd_ln_free_r(config);
}

//From continous.c (pocketsphinx default files)
void check_command(const char *words){
	char s[256];
	char* timeVar = "time";
	char* weatherVar = "weather";
	char* sportVar = "sport";
	char* sportsVar = "sport's";
	char* alarmVar = "alarm";
	strcpy(s, words); //tokenizer doens't work with const char
	char* token = strtok(s, " "); //divide the words up by spaces
	char* temptoken;
	int recognizedCommand = 0;
	while (token) {
	 printf("token: %s\n", token);
		if (strcmp(token,timeVar) == 0){
		 system("date '+%A %r' > date.txt");
	         system("espeak -f date.txt -s 135 2>/dev/null");
		 //printf("Do time stuff\n");
		 recognizedCommand = 1;
		} else if(strcmp(token,weatherVar) == 0){
		 getWeather();
		 system("espeak -f weather.txt -s 135 2>/dev/null");
		 //printf("Do weather stuff\n");
		 recognizedCommand = 1;
		} else if (strcmp(token,sportVar) == 0  || strcmp(token,sportsVar) == 0){
		//printf("Do sport stuff\n");
		//recognizedCommand = 1;
		} else if (strcmp(token,alarmVar) == 0){
		//printf("Do alarm stuff\n");
		//recognizedCommand = 1;
		} else if (strcmp(token, "hello") == 0){
		 system("espeak 'Hello.' -s 135 2>/dev/null");
		 recognizedCommand = 1;
		} else if (strcmp(token, "name") == 0){
		 system("espeak 'Sleepful personal sleeping assistant' -s 135 2>/dev/null");
		 recognizedCommand = 1;
		} else if (strcmp(token, "hi") == 0){
		 system("espeak 'Hi.' -s 135 2>/dev/null");
		 recognizedCommand = 1;
		} else if (strcmp(token, "hey") == 0){
		 system("espeak 'Hey.' -s 135 2>/dev/null");
		 recognizedCommand = 1;
		} else if (strcmp(token, "good") == 0){
		temptoken = strtok(NULL, " ");
			if ( strcmp(temptoken, "morning") == 0){
			 system("espeak 'Good morning.' -s 135 2>/dev/null");
			 recognizedCommand = 1;
			}
	}
	else{
	//do nothing
	}
	 token = strtok(NULL, " ");
	}
	if (recognizedCommand == 1){
		system("espeak 'No commands recognized.' -s 135 2>/dev/null");
	}
}



//From continuous.c
static void
recognize_from_microphone()
{
    ad_rec_t *ad;
    int16 adbuf[2048];
    uint8 utt_started, in_speech;
    int32 k;
    char const *hyp;

    if ((ad = ad_open_dev(cmd_ln_str_r(config, "-adcdev"),
                          (int) cmd_ln_float32_r(config,
                                                 "-samprate"))) == NULL)
        E_FATAL("Failed to open audio device\n");
    if (ad_start_rec(ad) < 0)
        E_FATAL("Failed to start recording\n");

    if (ps_start_utt(ps) < 0)
        E_FATAL("Failed to start utterance\n");
    utt_started = FALSE;
    E_INFO("Ready....\n");
    system("espeak 'Ready to take commands' -s 135 2>/dev/null");
    for (;;) {
        if ((k = ad_read(ad, adbuf, 2048)) < 0)
            E_FATAL("Failed to read audio\n");
        ps_process_raw(ps, adbuf, k, FALSE, FALSE);
        in_speech = ps_get_in_speech(ps);
        if (in_speech && !utt_started) {
            utt_started = TRUE;
            E_INFO("Listening...\n");
        }
        if (!in_speech && utt_started) {
            /* speech -> silence transition, time to start new utterance  */
            ps_end_utt(ps);
            hyp = ps_get_hyp(ps, NULL );
            if (hyp != NULL) {
                printf("%s\n", hyp);
				check_command(hyp);
                fflush(stdout);
            }

            if (ps_start_utt(ps) < 0)
                E_FATAL("Failed to start utterance\n");
            utt_started = FALSE;
            E_INFO("Ready....\n");
	   system("espeak 'Ready to take a command' -s 135 2>/dev/null");
        }
        sleep_msec(100);
    }
    ad_close(ad);
}

int main(int argc, char *argv[])
{ 
	//the following sets up the api to be of use for a microphone, the program will crash if you attempt to run this without using the '-inmic yes'
    	//command option, also use '-adcdev plughw:0' on the raspberry pi to select the proper device.
    system("espeak 'Hello I am just getting ready' -s 135 2>/dev/null"); //greeting the user
    getMemo();
    char const *cfg;
    ps_free(ps);
    config = cmd_ln_parse_r(NULL, cont_args_def, argc, argv, TRUE);

    /* Handle argument file as -argfile. */
    if (config && (cfg = cmd_ln_str_r(config, "-argfile")) != NULL) {
        config = cmd_ln_parse_file_r(config, cont_args_def, cfg, FALSE);
    }

    if (config == NULL || (cmd_ln_str_r(config, "-infile") == NULL && cmd_ln_boolean_r(config, "-inmic") == FALSE)) {
	E_INFO("Specify '-infile <file.wav>' to recognize from file or '-inmic yes' to recognize from microphone.\n");
        cmd_ln_free_r(config);
	return 1;
    }

    ps_default_search_args(config);
    ps = ps_init(config);
    if (ps == NULL) {
        cmd_ln_free_r(config);
        return 1;
    }
    recognize_from_microphone();
//    char *words = try_to_record();
  //  check_command(words);
  //  printf("Returned: %s\n", words);
    return 0;
}
