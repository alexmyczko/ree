
/*
syntax: fontdump videobiosdump.rom
scans for vga fonts of size 8x8, 8x14 and 8x16 and dumps them to files
which you can use in the console like this: setfont or consolechars -f 8x16.fnt
alex@aiei.ch

character 0 and 255 are "\0"
meaning we have to look for a pattern like this
\0\0\0\0\\0\0\0\0\ + 253*8 bytes + \0\0\0\0\\0\0\0\0\
if we want a 8x8 font
*/

#include <stdio.h>
#include <strings.h>

#define	CHARA 65

char* ID_FONT8="\0\0\0\0\0\0\0\0";
char* ID_FONT14="\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
char* ID_FONT16="\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

FILE* f;
unsigned char buffer[256*1024];
char filename[32];

long filesize(FILE* stream)
{
    long ret;
    long savedpos=ftell(stream);
    fseek(stream,0L,SEEK_END);
    ret = ftell(stream);
    fseek(stream,savedpos,SEEK_SET);
    return ret;
}
		  
long dd(long skip, long count)
{
    fseek(f,skip,SEEK_SET);
    return (fread(buffer,count,1,f));
}

void save(char* filename, long size, long offset)
{
    FILE* out;
    out=fopen(filename,"w");
    fwrite(buffer+offset,size,1,out);
    fclose(out);
}

void show(long offset, int character, int fontsize)
{
    int x,y;
    int bittab[8]={1,2,4,8,16,32,64,128};
    for(x=0; x<fontsize; x++) {
	for(y=0; y<8; y++) {
	    if ((buffer[offset+(character*fontsize)+x]) & bittab[y]) {
		printf("*");
	    } else {
		printf("-");
	    }
	}
	printf("\n");
    }
}

int checksumchar(long offset, int character, int fontsize)
{
    int x;
    int csum=0;
    for (x=0; x<fontsize; x++) {
	csum+=buffer[offset+(character*fontsize)+x];
    }
    return csum;
}

int main(int argc, char* argv[])
{
    long start,last,location,count;
    int a,b,s,size;

    if (argc<2) {
	printf("Syntax: fontdump video.rom\n");
    }

    f=fopen(argv[1],"r");
    if (f==NULL) {
	printf("could not open %s\n",argv[1]);
	return 1;
    }

    start=0;
    last=filesize(f);
    printf("size %d\n",last);

    for (a=start; a<=last; a++) { buffer[a]=0xff; }
    dd(0,last);
    for (a=start; a<=last; a++) {
        location=a;
	/*printf("[1GPlease wait, scanning... %d",location);*/

	if (bcmp(buffer+a,ID_FONT8,8)==0) {
	    if (bcmp(buffer+a+(255*8),ID_FONT8,8)==0) {
		if (a<=last-(256*8)) {
		    /*sprintf(filename,"%x.rom",location);*/
		    if ((checksumchar(a, CHARA, 8)!=0) && (checksumchar(a,32,8)==0)) {
    			printf("Found FONT8 at %ld\n",location);
			/*show(a,CHARA,8);*/
			/*sleep(1);*/
			save("8x8.fnt",8*256,a);
		    }
		}
	    }
	}

	if (bcmp(buffer+a,ID_FONT14,14)==0) {
	    if (bcmp(buffer+a+(255*14),ID_FONT14,14)==0) {
		if (a<=last-(256*14)) {
		    /*sprintf(filename,"%x.rom",location);
		    save(filename,size);*/
		    if ((checksumchar(a, CHARA, 14)!=0) && (checksumchar(a,32,14)==0)) {
    			printf("Found FONT14 at %ld\n",location);
			/*show(a,CHARA,14);*/
			/*sleep(1);*/
			save("8x14.fnt",14*256,a);
		    }
		}
	    }
	}

	if (bcmp(buffer+a,ID_FONT16,16)==0) {
	    if (bcmp(buffer+a+(255*16),ID_FONT16,16)==0) {
		if (a<=last-(256*16)) {
		    /*sprintf(filename,"%x.rom",location);*/
		    if ((checksumchar(a, CHARA, 16)!=0) && (checksumchar(a,32,16)==0)) {
    			printf("Found FONT16 at %ld\n",location);
			/*show(a,CHARA,16);*/
			/*sleep(1);*/
			save("8x16.fnt",16*256,a);
		    }
		}
	    }
	}

    }
    printf("\n");
    fclose(f);

    return 0;
}
