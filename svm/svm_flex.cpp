/*
   Copyright 2018 - The OPRECOMP Project Consortium, Alma Mater Studiorum
   Università di Bologna. All rights reserved.

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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "flexfloat.hpp"

#define FLOAT float

#define N 37
#define M 315

FLOAT data_model[N*M];

typedef struct _svm_node
{
        int index;
        float value;
} svm_node;

typedef struct _svm_parameter
{
        int svm_type;
        int kernel_type;
        int degree;     /* for poly */
        float gamma;    /* for poly/rbf/sigmoid */
        double coef0;   /* for poly/sigmoid */
} svm_parameter;

float kernel_function(const svm_node *x, const svm_node *y, const svm_parameter param)
{
    flexfloat<EXP_SVM_KERNEL_SUM, FRAC_SVM_KERNEL_SUM> sum = 0;

    // flexfloat_vectorization = true;
    while(x->index != -1 && y->index !=-1)
    {
        if(x->index == y->index)
        {
            flexfloat<EXP_SVM_KERNEL_D, FRAC_SVM_KERNEL_D> d = flexfloat<EXP_SVM_KERNEL_TEMP1, FRAC_SVM_KERNEL_TEMP1>(flexfloat<EXP_SVM_KERNEL_X, FRAC_SVM_KERNEL_X>(x->value)) - flexfloat<EXP_SVM_KERNEL_TEMP1, FRAC_SVM_KERNEL_TEMP1>(flexfloat<EXP_SVM_KERNEL_Y, FRAC_SVM_KERNEL_Y>(y->value));
            sum += d*d;
            ++x;
            ++y;
        }
        else
        {
            if(x->index > y->index)
            {
                sum += flexfloat<EXP_SVM_KERNEL_Y, FRAC_SVM_KERNEL_Y>(y->value) * flexfloat<EXP_SVM_KERNEL_Y, FRAC_SVM_KERNEL_Y>(y->value);
                ++y;
            }
            else
            {
                sum += flexfloat<FRAC_SVM_KERNEL_X, FRAC_SVM_KERNEL_X>(x->value) * flexfloat<FRAC_SVM_KERNEL_X, FRAC_SVM_KERNEL_X>(x->value);
                ++x;
            }
        }
    }
    // flexfloat_vectorization = false;

    while(x->index != -1)
    {
        sum += flexfloat<EXP_SVM_KERNEL_X, FRAC_SVM_KERNEL_X>(x->value) * flexfloat<EXP_SVM_KERNEL_X, FRAC_SVM_KERNEL_X>(x->value);
        ++x;
    }

    while(y->index != -1)
    {
        sum += flexfloat<EXP_SVM_KERNEL_Y, FRAC_SVM_KERNEL_Y>(y->value) * flexfloat<EXP_SVM_KERNEL_Y, FRAC_SVM_KERNEL_Y>(y->value);
        ++y;
    }

    return expf(float(-param.gamma*sum));
}



float svm_predict_values(const float *data_model, const svm_node *x, float *kvalue)
{

    int j;
    int i;

    int nr_class = 2;
    int l =  M;
    svm_node SV[N];
    svm_parameter _param = {0, 2, 0, 0.0277778f, 0};


    for(i=0; i < l; i++)
    {

        for(j=1; j<N; j++)
        {
            SV[j-1].index = j;
            SV[j-1].value = data_model[N*i+j];
        }
        SV[N-1].index = -1;
        SV[N-1].value = 0.0f;

        kvalue[i] = double(flexfloat<EXP_SVM_PREDICT_KVALUE, FRAC_SVM_PREDICT_KVALUE>(kernel_function(x, SV,_param)));
    }
}


int main()
{
	svm_node x1[N];
	FLOAT kvalue[M];

    srand(5497);
	int i;
	for (i=0; i<N-1; i++)
	{
		x1[i].index = i;
		x1[i].value = (FLOAT)rand() / (FLOAT)RAND_MAX;
	}
	x1[N-1].index = -1;
    x1[N-1].value = 0.0f;

    for (i=0; i<N*M; i++)
    {
        data_model[i] = (FLOAT)rand() / (FLOAT)RAND_MAX;
    }

	svm_predict_values(data_model, x1, kvalue);

    for(i = 0; i < M; i++)
	    printf("%.15f,", kvalue[i]);
     // print_flexfloat_stats();

	return 0;
}
