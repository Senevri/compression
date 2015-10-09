#include <assert.h> 


int rle_encode(unsigned char * dest, unsigned char * src, int len) {
    int i = 0;
    int repeats;
    unsigned char value;
    int dest_index = 0;
    while (i != len) {
        char compare;
        value = src[i];
        repeats = 0;
        compare = value;
        while (compare == value) {
            repeats++;
            if ((i + repeats) != len) {
                compare = src[i + repeats];
            }
            else {
                break;
            }
            if (repeats == 256) {
                break;
            }
        }
        //repeats;
        dest[dest_index] = src[i];
        dest[dest_index + 1] = repeats - 1;
        dest_index += 2;
        //i++;
        i += repeats;
    }
    return dest_index; /*size of dest*/
}


/*
 * decode: dest should be reserved to 256x length of decoded sequence. 
 */
int rle_decode(unsigned char * dest, unsigned char * src, int len) {
    int i = 0;
    int dest_index = 0;
    int repeats;
    unsigned char value;
    while (i < len) {
        value = src[i];
        repeats = src[i + 1];
        dest[dest_index] = value;
        dest_index++;
        while (repeats > 0) {
            assert(repeats != 0);
            dest[dest_index] = value;
            dest_index++;
            repeats--;
        }
        i += 2;
    }
    return dest_index; /*size of dest*/
}


/*
 * These versions indicate the use of an escape character to denote
 * when to apply rle.
 */
int rle_encode_ec(unsigned char * dest, unsigned char * src, int len) {
    int i = 0;
    int repeats;
    unsigned char value;
    int dest_index = 0;
    while (i != len) {
        char compare;
        value = src[i];
        repeats = 0;
        compare = value;
        //    printf("\np2");    
        while (compare == value) {
            repeats++;
            if ((i + repeats) != len) {
                compare = src[i + repeats];
            }
            else {
                break;
            }
            if (repeats == 256) {
                break;
            }
        }
        if (repeats > 1) {
            dest[dest_index] = src[i];
            dest[dest_index + 1] = src[i]; //repeat flag
            dest[dest_index + 2] = repeats - 2;
            dest_index += 3;
            i += repeats;
        }
        else {
            dest[dest_index] = src[i];
            dest_index++;
            i++;
        }
    }
    return dest_index; /*size of dest*/
}


/*
 * decode: dest should be reserved to 256x length of decoded sequence. 
 */
int rle_decode_ec(unsigned char * dest, unsigned char * src, int len) {
    int i = 0;
    int dest_index = 0;
    int repeats;
    unsigned char value, rflag;
    while (i < len) {
        value = src[i];
        rflag = src[i + 1];
        if (value == rflag) {
            repeats = src[i + 2];
            dest[dest_index] = value;
            dest[dest_index + 1] = value;
            dest_index += 2;
            while (repeats > 0) {
                assert(repeats != 0);
                dest[dest_index] = value;
                dest_index++;
                repeats--;
            }
            i += 3;
        }
        else {
            dest[dest_index] = value;
            dest_index++;
            i++;
        }
    }
    return dest_index; /*size of dest*/
}