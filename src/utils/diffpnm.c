#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>


int  (*outsymb)(int,FILE *), (*insymb)(FILE *), 
	outsymb1(int, FILE *), outsymb2(int, FILE *),
	insymb1(FILE *), insymb2(FILE *);

#define LINELEN 256
#define PGMNAME "diffpnm"

double drand();
long   lrand();
extern long ber_threshold;
void usage();
int read_pnm_header(FILE *infile, int *colsp, int *rowsp, int *maxsp, int *typep);


int main(int argc, char *argv[])
{
	int i, 
	    inb1, inb2,    /* input symbol size (read from pgm header) */
	    c1, c2,
	    noheader=0;
	int rows1, cols1, rows2, cols2,
	    type1, type2,
		maxs1,   /* max input value 1 */
		maxs2,  /* max input value 2 */
		tolerance = 0;  /* max allowed error */

	char line[LINELEN];
	long n = 0, outsize,
	     n1 = 0;
	FILE *infile1 = NULL,
	     *infile2 = NULL,
	     *outfile = stdout;
	char *infilename1 = NULL,
	     *infilename2 = NULL;
	double rmse = 0.0, psnr;
	int  wts[256];
	int seed=0;
	int d, maxdif, goteof=0;

	for ( i=1; i<argc; i++ ) {
	    if (argv[i][0]=='-') {
	    	switch( argv[i][1] ) {
		    case 'e':
			if ( sscanf(argv[i],"-e%d",&tolerance) != 1 ) {
			    usage();
			    exit(-1);
			}
			break;
		    default:
			usage();
			exit(-1);
		}
	    }
	    else {
		if ( infilename1 == NULL ) {
			infilename1 = argv[i];
		}
		else if ( infilename2 == NULL ) {
			infilename2 = argv[i];
		}
		else  {
			fprintf(stderr,"Too many params\n");
			exit(-1);
		}

	    }
	}


	if ( infilename1 != NULL ) {
		if ( (infile1=fopen(infilename1,"rb")) == NULL ) {
			perror(infilename1);
			exit(-1);
		}
	}
	else {
		fprintf(stderr,"Must specifiy at least one file name\n");
		usage();
		exit(-1);
	}
	if ( infilename2 != NULL ) {
		if ( (infile2=fopen(infilename2,"rb")) == NULL ) {
			perror(infilename2);
			exit(-1);
		}
	}
	else {
		infile2 = stdin;
	}


	if ( read_pnm_header(infile1, &cols1, &rows1, &maxs1, &type1) != 0 ) {
		fprintf(stderr, "bad header in %s\n",infilename1);
		exit(-1);
	}
	if ( read_pnm_header(infile2, &cols2, &rows2, &maxs2, &type2) != 0 ) {
		fprintf(stderr, "bad header in %s\n",(infilename2==NULL)?"Standard input":infilename2);
		exit(-1);
	}

	if ( type1 != type2 ) {
		fprintf(stderr,"File types are different: type1=%d type2=%d\n",
		type1, type2);
		exit(-1);
	}
	if ( cols1 != cols2 || rows1 != rows2 ) {
	    fprintf(stderr,"Images have different dimensions: cols1=%d rows1=%d cols2=%d rows2=%d\n",cols1,rows1,cols2,rows2);
	    exit(-1);
	}

	for ( inb1=8; (1L<<inb1)<=maxs1; inb1++);
	for ( inb2=8; (1L<<inb2)<=maxs2; inb2++);

	if ( (inb1<=8) != (inb2<=8) ) {
		fprintf(stderr,"Images have different symbol sizes\n");
		exit(-1);
	}

	if ( inb1 <= 8 ) 
		insymb = insymb1;
	else
		insymb = insymb2;


	maxdif = 0;

	while ( 1 ) {
	    c1 = insymb(infile1);
	    c2 = insymb(infile2);

	    if ( c1==EOF || c2==EOF ) {
		if ( c1 != c2 ) {
			fprintf(stderr,"EOF on %s\n",(c1==EOF)?"file 1":"file 2");
			goteof = 1;
		}
		break;
	    }
	    d = abs(c1-c2);
	    rmse += d*d;
	    if ( d > maxdif )
		maxdif = d;
	
	    n ++;
	}

	rmse = sqrt(rmse/n);
	printf("%ld symbols processed. Max difference = %d RMSE = %6.3lf",n, maxdif,rmse);
	if ( maxdif > 0 ) {
	    psnr = 20.0*log(maxs1/rmse)/log(10.0);
	    printf(" PSNR = %5.2lfdB\n",psnr);
	}
	else 
	    printf("\n");
	if ( maxdif == 0 && !goteof )
		printf("*** Images are identical\n");

	if ( maxdif > tolerance ) 
	    return -1;
	return 0;
}

void usage()
{
    fprintf(stderr,"usage: %s [-e<tolerance>] file1 file2\n",PGMNAME);
}


int
insymb1(FILE *inf)
{
	return getc(inf);
}

int 
insymb2(FILE *inf)
{
	register int c1, c2;

	c1=getc(inf);
	c2=getc(inf);

	return (c1<<8)|c2;
}

int
outsymb1(int c, FILE *outf)
{
	return putc(c, outf);
}

int
outsymb2(int c, FILE *outf)
{
	putc(c>>8, outf);
	return putc(c&0xff, outf);
}

int
read_pnm_header(FILE *infile, int *colsp, int *rowsp, int *maxsp, int *typep)
{
	int rows, cols, maxs, type;
	char line[LINELEN+1];

	while ( fgets(line,LINELEN,infile)!=NULL && line[0]=='#');
	if ( ( sscanf(line,"P%d",&type) != 1 ) 
		|| ( type != 5 && type != 6 && type != 7 ) ) {
		fprintf(stderr,"bad header: type\n");
		return -1;
	}
	while ( fgets(line,LINELEN,infile)!=NULL && line[0]=='#');
	if ( sscanf(line,"%d %d",&cols,&rows) != 2 ) {
		fprintf(stderr,"bad header: cols rows\n");
		return -1;
	}
	while ( fgets(line,LINELEN,infile)!=NULL && line[0]=='#');
	if ( sscanf(line,"%d",&maxs) != 1 ) {
		fprintf(stderr,"bad header: maxs\n");
		return -1;
	}

	*colsp = cols;
	*rowsp = rows;
	*maxsp = maxs;
	*typep = type;
	return 0;
}
