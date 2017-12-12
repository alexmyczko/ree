
/*
gcc ree.c -o ree.bin

rom extension extractor (ported to shell from the pascal version of 1997)
(and now it is .c, and like 69 times faster or so)
see http://www.firmware.com/support/bios/romext.htm for more info
extracts system, scsi, video bios
gurkan@linuks.mine.nu, www.linuks.mine.nu

a checksum of 0 is ok! any other number indicates it is either not a rom
extension or the rom is defective

scan from c0000 - f0000 in 512 byte blocks
Uª{l}{code}
l*512=length of the code including headers
*/

#include <stdio.h>
#include <strings.h>

char* ID_ROMEXT="Uª"; /* 0x55aa */

FILE* f;
unsigned char buffer[256*1024];
char filename[32];

void dd(long skip, long count)
{
    fseek(f,skip,SEEK_SET);
    fread(buffer,count,1,f);
}

void save(char* filename,long size)
{
    FILE* out;
    out=fopen(filename,"w");
    fwrite(buffer,size,1,out);
    fclose(out);
}

unsigned char checksum(long size)
{
    unsigned sum=0;
    int c;
    for (c=0; c<size; c++) sum+=buffer[c];
    return (sum);
}

int main(int argc, char* argv[])
{
    long start,last,location,count;
    int a,b,s,size;

    f=fopen("/dev/mem","r");
    if (f==NULL) {
	printf("could not open /dev/mem\n");
	printf("(are you root?)\n");
	return 1;
    }

    start=0xc0000;
    last=0xf0000;

    for (a=start; a<=last; a+=512) {
	dd(a,3);

        location=a;
	printf("[1GPlease wait, scanning... %x",location);
    
	if (strncmp(buffer,ID_ROMEXT,2)==0) {
    	    size=buffer[2]*512;
	    dd( a,size );
    	    printf("\nFound something at %x (%d bytes) (checksum %d)\n",location,size,checksum(size));
	    sprintf(filename,"%x.rom",location);
	    save(filename,size);
	}
    }

    /* save bios rom */
    /* start=0xf0000;
    count=1*65536;
    dd( start,count );
    printf("\nFound system bios at F0000 (65536) (checksum %d)\n",checksum(count));
    sprintf(filename,"%x.rom",start);
    save(filename,count); */
    printf("\n");

    fclose(f);

    return 0;
}
