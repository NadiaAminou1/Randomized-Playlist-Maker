#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SONGS 100
#define MAX_INFO_LENGTH  150
#define MAX_NAME_LENGTH 50
#define MAX_SONG_LENGTH 100
#define DEFAULT_INPUT_FILE "artistes+songs.txt"


/**
	read songs from file
*/
int readSongsFromFile(FILE* fp, char artiste_song_time[MAX_SONGS][MAX_INFO_LENGTH]) ;

/**
	read songs from keyboard
*/
int readSongsFromKeyboard(char artiste_song_time[MAX_SONGS][MAX_INFO_LENGTH]);

/**
Sort the artistes and songs
*/
void insertion_sort(char artiste_song_time[MAX_SONGS][MAX_INFO_LENGTH], int n) ;

/**
	split string as specific string.
*/
char* splitString(char song[MAX_INFO_LENGTH], char* delim, int index) ;

/*
 	print out the sorted artistes and songs
 */
int print_artistes_songs_times(char artiste_song_time[MAX_SONGS][MAX_INFO_LENGTH], int numSongs) ;

/*
Swap elements i and j in array arr of ints.
*/
void swapArrElems(int arr[], int i, int j);

/*
Shuffle an array of ints using the Fisher-Yates Durstenfeld Knuth Shuffle algorithm.
*/
void fydkShuffle1(int arr[], int size);

/**
	convert time string to second
*/
int convertString2Second(char* string) ;

/**
	convert second to string
*/
char* convertSecond2String(int second) ;

/*
	generate and print out the playlist.
*/
int gen_playlist(char artiste_song_time[MAX_SONGS][MAX_INFO_LENGTH], int numSongs) ;

/**
initiate string with empty
*/
int initString(char* buff, int length) ;

/**
    convert int to string
*/
char *my_itoa(int num, char *str);

/**
    convert string to int
*/
int my_atoi(char * str);
//main
int main(int argc, char** argv)
{

    FILE* input_fp;
    char artiste_song_time[MAX_SONGS][MAX_INFO_LENGTH];
    int numSongs;

    /* Check if we have an argument. */
    if (argc > 1)
    {
        if ((input_fp = fopen((const char *)argv[1], "r")) == NULL)
        {
            printf("%s does not exist or cannot be opened for reading.\n", argv[1]);
            return 1;
        }
        else
        {
            numSongs = readSongsFromFile(input_fp, artiste_song_time);
            fclose(input_fp);
        }
    }
    else
    {
        /* No file supplied; try the default. */
        if ((input_fp = fopen(DEFAULT_INPUT_FILE, "r")) == NULL)
        {
            /* Default file isn't there. Enter from the keyboard. */
            printf("%s: No file found.\n", DEFAULT_INPUT_FILE);
            printf("Enter from the keyboard.\n");
            numSongs = readSongsFromKeyboard(artiste_song_time);
        }
        else
        {
            numSongs = readSongsFromFile(input_fp, artiste_song_time);
            fclose(input_fp);
        }
    }
    /* Sort the artistes and songs, ... */
    insertion_sort(artiste_song_time, numSongs);
    /* ... print out the sorted artistes and songs ... */
    print_artistes_songs_times(artiste_song_time, numSongs);
    /* ... and generate and print out the playlist. */
    gen_playlist(artiste_song_time, numSongs);
    return 0;
}
/**
	read songs from file
*/
int readSongsFromFile(FILE* fp, char artiste_song_time[MAX_SONGS][MAX_INFO_LENGTH]) {

    char buff[MAX_INFO_LENGTH] = { '\0' };
    char artistName[MAX_NAME_LENGTH] = { '\0' };
    int numSongs = 0;
    int isNewArtist = 0;
    int index = 0;
    while (fgets(buff, MAX_INFO_LENGTH - 1, fp))
    {
        if (index == 0) {//if index is 0, buff is artist name
            strcpy(artistName, buff);
            //remove trailing new line
            artistName[strcspn(artistName, "\n")] = 0;
            index++;
            continue;
        }
        if (strcmp(buff, "\n") == 0) {//appear new artist
            isNewArtist = 1;
            //set song number of any artist as 0
            index = 0;
            initString(artistName, 50);
            continue;
        }
        // Remove trailing newline
        buff[strcspn(buff, "\n")] = 0;
        char artistSong[MAX_INFO_LENGTH] = { '\0' };
        strcat(artistSong, artistName);
        //add * to end of name
        strcat(artistSong, "*");
        //add song's information to artist name
        strcat(artistSong, buff);
        strcpy(artiste_song_time[numSongs], artistSong);//artist information will be shown as "artist name*song title***song duration"

        //init string as empty.
        initString(buff, MAX_INFO_LENGTH);
        numSongs++;
    }
    return numSongs;
}

/**
	read songs from keyboard
*/
int readSongsFromKeyboard(char artiste_song_time[MAX_SONGS][MAX_INFO_LENGTH]) {

    char artisteName[MAX_NAME_LENGTH] = { '\0' };
    char song[MAX_SONG_LENGTH] = { '\0' };
    char isEnd[3] = "n";
    int isNewArtiste;
    int numSongs = 0;
    while (1) {

        //get name from keyboard
        printf("Enter artiste name:");

        scanf("%s", artisteName);

        //get song information as type "song title*song duration"
        printf("Enter song name and duration(name*duration):");
        //gets(song);
        scanf("%s", song);
        char songs[MAX_INFO_LENGTH] = { '\0' };
        strcat(songs, artisteName);
        strcat(songs, "*");
        strcat(songs, song);
        //append new song information to array
        strcpy(artiste_song_time[numSongs], songs);
        //ask whether end or no
        printf("Are you finish to enter value? y/n :");
        scanf("%s", isEnd);
        if (strcmp(isEnd, "y") == 0) {
            break;
        }
        else {
            numSongs++;
        }
        initString(artisteName, MAX_NAME_LENGTH);
        initString(song, MAX_SONG_LENGTH);
    }
    return numSongs;
}

/**
Sort the artistes and songs
*/
void insertion_sort(char artiste_song_time[MAX_SONGS][MAX_INFO_LENGTH], int n) {
    int i = 0;
    int j = 0;
    char swap[MAX_INFO_LENGTH] = { '\0' };
    for (i = 1; i < n; i++) {
        strcpy(swap, artiste_song_time[i]);
        j = i - 1;

        while (strcmp(swap, artiste_song_time[j]) < 0 && j >= 0) {
            strcpy(artiste_song_time[j +  1], artiste_song_time[j]);
            j --;
        }
        strcpy(artiste_song_time[j + 1], swap);

    }
}

/**
	split string as specific string.
*/
char* splitString(char song[MAX_INFO_LENGTH], char* delim, int index) {

    char inputString[MAX_INFO_LENGTH] = {'\0'};
    char token[MAX_INFO_LENGTH] = { '\0' };

    int num = 0;
    //copy input string to local variable
    strcpy(inputString, song);
    for (strcpy(token, strtok(inputString, delim)); token; strcpy(token, strtok(NULL, delim)))
    {
        if (num == index) {
            char * ret = token;
            return ret;
        }
        num++;
    }
}
/* ... print out the sorted artistes and songs ... */
int print_artistes_songs_times(char artiste_song_time[MAX_SONGS][MAX_INFO_LENGTH], int numSongs) {
    printf("Sortest list of artistes and songs:\n");
    char artiste[MAX_INFO_LENGTH] = { '\0' };
    char songTitle[MAX_INFO_LENGTH] = { '\0' };
    char songDuration[MAX_INFO_LENGTH] = { '\0' };
    int i = 0;
    for (i = 0; i < numSongs; i++) {
        char artisteBuff[MAX_INFO_LENGTH] = { '\0' };
        //get artiste, song title and song duration from information
        strcpy(artisteBuff,splitString(artiste_song_time[i], "*", 0));
        strcpy(songTitle, splitString(artiste_song_time[i], "*", 1));
        strcpy(songDuration, splitString(artiste_song_time[i], "*", 2));
        //print information
        if (strcmp(artiste, artisteBuff) == 0) {
            printf("  %s***%s\n", songTitle, songDuration);
        }
        else {
            strcpy(artiste, artisteBuff);
            printf("%s\n", artiste);
            printf("  %s***%s\n", songTitle, songDuration);
        }
    }
    return 0;
}

/*
Swap elements i and j in array arr of ints.
*/
void swapArrElems(int arr[], int i, int j)
{
    int buff = arr[i];
    arr[i] = arr[j];
    arr[j] = buff;
}
/*
Shuffle an array of ints using the Fisher-Yates Durstenfeld Knuth Shuffle algorithm.
*/
void fydkShuffle1(int arr[], int size)
{
    int i, j;
    //srand(time(NULL));
    for (i = size - 1; i >= 0; i--)
    {
        j = rand() % (i + 1);

        swapArrElems(arr, i, j);
    }
    return;
}

/**
	convert time string to second
*/
int convertString2Second(char* string) {
    char buff[6] = {'\0'};
    char minute[3] = { '\0' };
    char second[3] = {'\0'};
    int ret = 0;
    strcpy(buff, string);
    strcpy(minute, splitString(buff, ":", 0));
    strcpy(second, splitString(buff, ":", 1));
    ret = my_atoi(minute) * 60 + my_atoi(second);
    return ret;
}

/**
	convert second to string
*/
char* convertSecond2String(int second) {
    int nminute = second / 60;
    int nsecond = second % 60;
    char sminute[3];
    char ssecond[3];
    char result[6] = {'\0'};
    char * ret;
    my_itoa(nminute, sminute);
    my_itoa(nsecond, ssecond);
    strcat(result, sminute);
    strcat(result, ":");
    strcat(result, ssecond);
    ret = result;
    return ret;
}
/*
	generate and print out the playlist.
*/
int gen_playlist(char artiste_song_time[MAX_SONGS][MAX_INFO_LENGTH], int numSongs) {
    int indexArray[MAX_SONGS];
    char suffledList[MAX_SONGS][MAX_INFO_LENGTH];
    int suffledIndex = 0;
    int totalTime = 0;
    char strTotalTime[6] = { '\0' };
    int songIndex = 0;
    char currentArt[MAX_NAME_LENGTH] = { '\0' };
    printf("Randomized playlist\n");
    //init index array for suffle
    int i;
    for (i = 0; i < numSongs; i++) {
        indexArray[i] = i;
    }
    //suffle index
    fydkShuffle1(indexArray, numSongs);
    //get randomize playlist
    while ((songIndex < numSongs)) {
        int j = indexArray[songIndex];

        int isAlreadyExist = 0;
        int numSongsofArtiste = 0;
        //check whether there is same song or not
        int k;
        for (k = 0; k < songIndex; k++) {
            if (strcmp(suffledList[k], artiste_song_time[j]) == 0) {
                isAlreadyExist = 1;
                break;
            }
        }
        //if no exist, append to list
        if (isAlreadyExist == 0) {
            //check whether an artiste have more than two songs in succession or not
            if (suffledIndex >= 2) {
                char currArt[MAX_NAME_LENGTH] = { '\0' };
                char prevArt[MAX_NAME_LENGTH] = { '\0' };
                char preprevArt[MAX_NAME_LENGTH] = { '\0' };
                //get current, preview and prepreview artiste name
                strcpy(currArt, splitString(artiste_song_time[j], "*", 0));
                strcpy(prevArt, splitString(suffledList[suffledIndex - 1], "*", 0));
                strcpy(preprevArt, splitString(suffledList[suffledIndex - 2], "*", 0));
                //check
                if ((strcmp(preprevArt, currArt) == 0) && (strcmp(prevArt, currArt) == 0)) {
                    songIndex++;
                    continue;
                }
                else {
                    //check whether  an artiste have more than three songs in total in the playlist or not
                    for (k = 0; k < suffledIndex; k++) {
                        char* buff = splitString(suffledList[k], "*", 0);
                        if (strcmp(buff, currArt) == 0) {
                            numSongsofArtiste++;
                        }
                    }
                    if (numSongsofArtiste > 2) {
                        songIndex++;
                        numSongsofArtiste = 0;
                        continue;
                    }
                }
            }

            //calculate total time
            totalTime = totalTime + convertString2Second(splitString(artiste_song_time[j], "*", 2));
            if (totalTime >= 3600) {
                break;
            }
            strcpy(strTotalTime, convertSecond2String(totalTime));
            //append current song information to suffled list for music player
            strcpy(suffledList[suffledIndex], artiste_song_time[j]);
            songIndex++;
            suffledIndex++;
        }
        else {
            songIndex++;
        }
    }
    //print randomized playlist
    for (i = 0; i < suffledIndex; i ++) {
        char name[MAX_NAME_LENGTH] = { '\0' };
        char title[MAX_SONG_LENGTH] = { '\0' };
        char duration[MAX_SONG_LENGTH] = { '\0' };
        //get name, song title and duration
        strcpy(name, splitString(suffledList[i], "*", 0));
        strcpy(title, splitString(suffledList[i], "*", 1));
        strcpy(duration, splitString(suffledList[i], "*", 2));
        printf("%s : %c%s%c %c%s%c\n", name, '"', title, '"', '(', duration, ')');
    }
    printf("\nTotal duration:%s \n", strTotalTime);
    return 0;
}

/**
initiate string with empty
*/
int initString(char* buff, int length) {
    int ret = 0;
    int i;
    for (i = 0; i < length; i++) {
        buff[i] = '\0';
        ret++;
    }
    return ret;
}
/**
    convert int to string
*/
char *my_itoa(int num, char *str)
{
    if(str == NULL)
    {
        return NULL;
    }
    sprintf(str, "%d", num);
    return str;
}

/**
    convert string to int
*/
int my_atoi(char * str){
    int a = 0;
    int i = 0;
    for (i = 0  ; str[i] != '\0'; i++) {
        a = (int)(str[i] - 48) + a * 10;
    }
    return a;
}
