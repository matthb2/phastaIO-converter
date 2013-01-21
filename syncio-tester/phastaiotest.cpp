#include <vector>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <dirent.h>
#include <cstring>
#include "phastaIO.h"
#define OMPI_SKIP_MPICXX 1
#include <mpi.h>

//Provided by phastaIO:
void Gather_Headers( int* fileDescriptor, std::vector< std::string >& headers );
int setIOparam(int myrank, int nprocs );

using namespace std;

int main(int argc, char** argv)
{

	int N_restart, nppf, N_files;
	int iarray[10];
	char* iformat="binary";
	int ione=1;
	int itwo=2;
	int igeombc;
	int rank;	
	int numprocs;
	int numfiles;
	int color;
	int nfields;
	char* filename;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);

	numfiles = setIOparam(rank, numprocs);
	color = computeColor(rank+1,numprocs, numfiles);

	asprintf(&filename, "%d-procs_case/geombc-dat.%d", numprocs, color);
	queryphmpiio(filename, &nfields, &nppf);
	initphmpiio(&nfields, &nppf, &numfiles, &igeombc, "read");
	openfile( filename, "read", &igeombc );
	free(filename);

	vector< string > headers;
	Gather_Headers( &igeombc, headers );

	readheader( &igeombc, "number of interior elements", (void*)iarray,
	     &ione, "integer", iformat );
	readheader( &igeombc, "keyword xadj", (void*)iarray,
	     &itwo, "integer", iformat );
	closefile(&igeombc, "read");
}

int setIOparam(int myrank, int nprocs )
{
  int count;
  int nfields;
  int nppf;
  int stepno;
  int part;
  char fname[255];
  DIR *d;
  struct dirent *filename;
  char* starting_dir;
  asprintf(&starting_dir,"%d-procs_case", nprocs);
  chdir(starting_dir); 
//Number of geombc files
  if(myrank == 0){
    if( (d = opendir(".")) != NULL) {
      count=0;
      while(filename=readdir(d)) {
      //printf("%s\n", pent->d_name);
        if(strncmp(filename->d_name,"geombc-dat",10)==0) {
          count=count+1;
        }
      }
      closedir(d);
    }
    else {
      printf("ERROR when counting geombc-dat\n");
    }
  }

  MPI_Bcast( &count, 1, MPI_INT, 0, MPI_COMM_WORLD );
  //printf("Here we gogo: %d %d\n",workfc.myrank,count);

  if(myrank == 0) {
    printf("Number of geombc-dat and restart-dat files to read: %d\n", count);
  }
  free(starting_dir);
  return(count+1);

}

