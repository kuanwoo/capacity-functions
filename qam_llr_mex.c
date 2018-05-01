/*
 * qam_llr_mex.c - Compute LLRs for QAM
 *
 * Usage: qam_llr_mex(C, sigma2, y, Pk)
 * C     :=   Complex constellation in Gray-mapping order
 * sigma2:=   Noise variance per each constellation point
 * y     :=   Received complex symbols
 * Pk    :=   Probability of each constellation symbol
 *
 * Use this function to compute log-likelihood-ratios
 * for M-QAM constellations.
 *
 * Compile with: mex -lm -R2018a qam_llr_mex.c
 * (requires MATLAB R2018a or newer versions)
 * Works under 64-bit Linux. Don't know/care under other OSs.
 *
 * 2018 - Dario Pilori <dario.pilori@polito.it>
 */
#include <math.h>
#include <complex.h>
#include <omp.h>
#include "capacity_functions.h"
#include "mex.h"

/* Gateway function */
void mexFunction(int nlhs, mxArray *plhs[],
        int nrhs, const mxArray *prhs[])
{
    size_t M, Ns;                   /* constellation and data size */
    double complex *C, *y;          /* Data and constellation */
    double *l, *Pk, *s2;                 
    
    /* Verify input */
    if(nrhs != 4) {
        mexErrMsgIdAndTxt("DspLibrary:qam_gmi_mex:nrhs",
                "Four inputs required.");
    }
    
    /* Verify output */
    if(nlhs > 1) {
        mexErrMsgIdAndTxt("DspLibrary:qam_gmi_mex:nlhs",
                "Max one output.");
    }
    
    /* Get sizes */
    M = mxGetM(prhs[0]);
    Ns = mxGetM(prhs[2]);
    
    /* Get noise variance */
    s2 = mxGetDoubles(prhs[1]);
        
    /* Get constellation and received data */
    C = (double complex *) mxGetComplexDoubles(prhs[0]);
    y = (double complex *) mxGetComplexDoubles(prhs[2]);
    
    /* Get probabilities */
    Pk = mxGetDoubles(prhs[3]);
    
    /* Allocate the output matrix */
    plhs[0] = mxCreateDoubleMatrix((mwSize) (Ns*log2(M)),1,mxREAL);
    
    /* get a pointer to the output matrix */
    l = mxGetDoubles(plhs[0]);
        
    /* Call function */
    qam_soft_decode(y, Ns, C, Pk, M, s2, l);
}
