#include <pocketsphinx.h>
#include <time.h>

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

void try_to_record(){
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
    ps_free(ps);
    cmd_ln_free_r(config);
}

void check_command(char *words){
    size_t len = strlen(words);
    printf("%d\n", len);
    int i = 0;
   while(i < (int) len){
	printf("%c\n", words[i]);
   i++;
   }


}

int main(int argc, char *argv[])
{
    char *words = play_goforward();
    check_command(words);
    printf("Returned: %s\n", words);
    return 0;
}
