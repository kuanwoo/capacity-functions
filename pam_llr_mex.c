/*
 * pam_llr_mex.c - Compute LLRs for PAM
 *
 * Use this function to compute log-likelihood-ratios
 * for M-PAM constellations.
 *
 * Compile with: mex -lm -R2018a qam_llr_mex.c
 * (requires MATLAB R2018a or newer versions)
 *
 * 2018 - Dario Pilori <dario.pilori@polito.it>
 */
#include <math.h>
#include "capacity_functions.h"
#include <omp.h>
#include "mex.h"

/* Gateway function */
void mexFunction(int nlhs, mxArray *plhs[],
        int nrhs, const mxArray *prhs[])
{
    size_t M, Ns;                   /* constellation and data size */
    double *C, *y, *l;
    double snr, Es, sigma;
    double *Pk;
    int m, i;
    
    /* Verify input */
    if(nrhs != 3) {
        mexErrMsgIdAndTxt("dsp-library:qam_gmi_mex:nrhs",
                "Three inputs required.");
    }
    
    /* Verify output */
    if(nlhs > 1) {
        mexErrMsgIdAndTxt("dsp-library:qam_gmi_mex:nlhs",
                "Max one output.");
    }
    
    /* Get sizes */
    M = mxGetM(prhs[0]);
    Ns = mxGetM(prhs[2]);
    m = log2(M);
    
    /* Get SNR */
    snr = mxGetScalar(prhs[1]);
        
    /* Get constellation and received data */
    C = (double *) mxGetDoubles(prhs[0]);
    y = (double *) mxGetDoubles(prhs[2]);
    
    /* Allocate the output matrix */
    plhs[0] = mxCreateDoubleMatrix((mwSize) (Ns*m),1,mxREAL);
    
    /* get a pointer to the output matrix */
    l = mxGetDoubles(plhs[0]);
    
    /* Calculate symbol energy */
    Pk = (double *)mxMalloc((mwSize) sizeof(double)*M);
    for(i=0; i<M; i++) {
      Pk[i] = 1.0 / M;
    }
    Es = symbol_energy(C, Pk, M);
    mxFree(Pk);
    
    /* Call function */
    sigma = sqrt(Es) * pow(10,-snr/20);
    pam_soft_decode(y, Ns, C, M, sigma, l);
}

