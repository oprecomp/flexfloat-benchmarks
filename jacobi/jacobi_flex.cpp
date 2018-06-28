/*
   Copyright 2018 - The OPRECOMP Project Consortium, Alma Mater Studiorum
   Università di Bologna, Consorzio Interuniversitario CINECA.
   All rights reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <string.h>

#include "flexfloat.hpp"

// Grid boundary conditions
#define RIGHT 1.0
#define LEFT 1.0
#define TOP 1.0
#define BOTTOM 10.0

// Algorithm settings
#define TOLERANCE 0.0001
#define NPRINT 1000
#define MAX_ITER 100000

int main(int argc, char*argv[]) {

  int k;

  flexfloat<EXP_TMPNORM, FRAC_TMPNORM> tmpnorm;
  flexfloat<EXP_BNORM, FRAC_BNORM> bnorm;
  flexfloat<EXP_NORM, FRAC_NORM> norm;

  int nx=32;
  int ny=32;
  int ny2=ny+2;
  int nthds=1;

  double *grid= (double*)malloc(sizeof(double)*(nx+2)*(ny+2));
  double *grid_new= (double*)malloc(sizeof(double)*(nx+2)*(ny+2));
  double *temp= (double*)malloc(sizeof(double)*(nx+2)*(ny+2));

  // Initialise grid boundaries
  int i,j;
  for (i=0;i<ny+2;i++) {
    grid[i]=double(flexfloat<EXP_GRID, FRAC_GRID>(TOP));
    grid_new[i]=double(flexfloat<EXP_GRID_NEW, FRAC_GRID_NEW>(grid[i]));
    
    j=(ny+2)*(nx+1)+i;
    grid[j]=double(flexfloat<EXP_GRID, FRAC_GRID>(BOTTOM));
    grid_new[j]=double(flexfloat<EXP_GRID_NEW, FRAC_GRID_NEW>(grid[j]));
  }

  for (i=1;i<nx+1;i++) {
    j=(ny+2)*i;
    grid[j]=double(flexfloat<EXP_GRID, FRAC_GRID>(LEFT));
    grid_new[j]=double(flexfloat<EXP_GRID_NEW, FRAC_GRID_NEW>(grid[j]));
    grid[j+ny+1]=double(flexfloat<EXP_GRID, FRAC_GRID>(RIGHT));
    grid_new[j+ny+1]=double(flexfloat<EXP_GRID_NEW, FRAC_GRID_NEW>(grid[j+ny+1]));
  }
   
  // Initialise rest of grid
  for (i=1; i<=nx; i++) 
    for (j=1; j<=ny; j++)
    {
      k=(ny+2)*i+j;
      grid[k]=double(flexfloat<EXP_GRID, FRAC_GRID>(0.0));
      grid_new[k]=double(flexfloat<EXP_GRID_NEW, FRAC_GRID_NEW>(grid[k]));
    }
   
  // clear_flexfloat_stats();

  tmpnorm=flexfloat<EXP_TMPNORM, FRAC_TMPNORM>(0.0);

  for (i=1;i<=nx;i++) {
    for (j=1;j<=ny;j++) {
      k=(ny+2)*i+j;            
      // EXPRESSION: tmpnorm=tmpnorm+pow(grid[k]*4-grid[k-1]-grid[k+1] - grid[k-(ny+2)] - grid[k+(ny+2)], 2);
      tmpnorm = flexfloat<EXP_TMPNORM, FRAC_TMPNORM>(
                  flexfloat<EXP_TEMP6, FRAC_TEMP6>(tmpnorm) +
                  flexfloat<EXP_TEMP6, FRAC_TEMP6>(
                    pow(double(
                      flexfloat<EXP_TEMP5, FRAC_TEMP5>(
                        flexfloat<EXP_TEMP4, FRAC_TEMP4>(
                          flexfloat<EXP_TEMP3, FRAC_TEMP3>(
                            flexfloat<EXP_TEMP2, FRAC_TEMP2>(
                              flexfloat<EXP_TEMP1, FRAC_TEMP1>(flexfloat<EXP_GRID, FRAC_GRID>(grid[k])) *
                              flexfloat<EXP_TEMP1, FRAC_TEMP1>(flexfloat<EXP_CONST1, FRAC_CONST1>(4))
                            ) -
                            flexfloat<EXP_TEMP2, FRAC_TEMP2>(flexfloat<EXP_GRID, FRAC_GRID>(grid[k-1]))
                          ) -
                          flexfloat<EXP_TEMP3, FRAC_TEMP3>(flexfloat<EXP_GRID, FRAC_GRID>(grid[k+1]))
                        ) -
                        flexfloat<EXP_TEMP4, FRAC_TEMP4>(flexfloat<EXP_GRID, FRAC_GRID>(grid[k-(ny+2)]))
                      ) -
                      flexfloat<EXP_TEMP5, FRAC_TEMP5>(flexfloat<EXP_GRID, FRAC_GRID>(grid[k+(ny+2)]))
                    ), 2)
                  )
                );
    }
  }
  bnorm = flexfloat<EXP_BNORM, FRAC_BNORM>(sqrt(double(tmpnorm)));

   //    MAIN LOOP 
  int iter;
  for (iter=0; iter<MAX_ITER; iter++) {

        tmpnorm=flexfloat<EXP_TMPNORM, FRAC_TMPNORM>(0.0);

        // flexfloat_vectorization = true;
        for (i=1;i<=nx;i++) {
         for (j=1;j<=ny;j++) {
          k=(ny+2)*i+j;
          // EXPRESSION: tmpnorm=tmpnorm+pow(grid[k]*4-grid[k-1]-grid[k+1] - grid[k-(ny+2)] - grid[k+(ny+2)], 2); 
          tmpnorm = flexfloat<EXP_TMPNORM, FRAC_TMPNORM>(
                      flexfloat<EXP_TEMP12, FRAC_TEMP12>(tmpnorm) +
                      flexfloat<EXP_TEMP12, FRAC_TEMP12>(
                        pow(double(
                          flexfloat<EXP_TEMP11, FRAC_TEMP11>(
                            flexfloat<EXP_TEMP10, FRAC_TEMP10>(
                              flexfloat<EXP_TEMP9, FRAC_TEMP9>(
                                flexfloat<EXP_TEMP8, FRAC_TEMP8>(
                                  flexfloat<EXP_TEMP7, FRAC_TEMP7>(flexfloat<EXP_GRID, FRAC_GRID>(grid[k])) *
                                  flexfloat<EXP_TEMP7, FRAC_TEMP7>(flexfloat<EXP_CONST1, FRAC_CONST2>(4))
                                ) -
                                flexfloat<EXP_TEMP8, FRAC_TEMP8>(flexfloat<EXP_GRID, FRAC_GRID>(grid[k-1]))
                              ) -
                              flexfloat<EXP_TEMP9, FRAC_TEMP9>(flexfloat<EXP_GRID, FRAC_GRID>(grid[k+1]))
                            ) -
                            flexfloat<EXP_TEMP10, FRAC_TEMP10>(flexfloat<EXP_GRID, FRAC_GRID>(grid[k-(ny+2)]))
                          ) -
                          flexfloat<EXP_TEMP11, FRAC_TEMP11>(flexfloat<EXP_GRID, FRAC_GRID>(grid[k+(ny+2)]))
                        ), 2)
                      )
                    );
          }
        }
        //  flexfloat_vectorization = false;
       
        norm=flexfloat<EXP_NORM, FRAC_NORM>( flexfloat<EXP_TEMP13, FRAC_TEMP13>(sqrt(double(tmpnorm))) / flexfloat<EXP_TEMP13, FRAC_TEMP13>(bnorm));

        if (norm < flexfloat<EXP_NORM, FRAC_NORM>(TOLERANCE)) break;
        
        // flexfloat_vectorization = true;
        for (i=1;i<=nx;i++) {
          for (j=1;j<=ny;j++) {
            k=(ny+2)*i+j;    
    	      // EXPRESSION: grid_new[k]=0.25 * (grid[k-1]+grid[k+1] + grid[k-(ny+2)] + grid[k+(ny+2)]);
            grid_new[k] = double(
                            flexfloat<EXP_TEMP17, FRAC_TEMP17>(flexfloat<EXP_CONST3, FRAC_CONST3>(0.25)) *
                            flexfloat<EXP_TEMP17, FRAC_TEMP17>(
                              flexfloat<EXP_TEMP16, FRAC_TEMP16>(
                                flexfloat<EXP_TEMP15, FRAC_TEMP15>(
                                  flexfloat<EXP_TEMP14, FRAC_TEMP14>(flexfloat<EXP_GRID, FRAC_GRID>(grid[k-1])) +
                                  flexfloat<EXP_TEMP14, FRAC_TEMP14>(flexfloat<EXP_GRID, FRAC_GRID>(grid[k+1]))
                                ) + 
                                flexfloat<EXP_TEMP15, FRAC_TEMP15>(flexfloat<EXP_GRID, FRAC_GRID>(grid[k-(ny+2)]))
                              ) +
                              flexfloat<EXP_TEMP16, FRAC_TEMP16>(flexfloat<EXP_GRID, FRAC_GRID>(grid[k+(ny+2)]))
                            )
                          );
          }
        }
        // flexfloat_vectorization = false;
        memcpy(temp, grid_new, sizeof(double) * (nx + 2) * (ny+2));
        memcpy(grid_new, grid, sizeof(double) * (nx + 2) * (ny+2));
        memcpy(grid, temp, sizeof(double) * (nx + 2) * (ny+2));
  }

  printf("%.15f,", double(norm));

  // print_flexfloat_stats();
  // printf("Terminated on %d iterations, Relative Norm=%e \n", iter, double(norm));

  free(grid);
  free(temp);
  free(grid_new);

  return 0;
}
