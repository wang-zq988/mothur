//
//  lsabundance.cpp
//  Mothur
//
//  Created by Sarah Westcott on 5/16/19.
//  Copyright © 2019 Schloss Lab. All rights reserved.
//

#include "lsabundance.hpp"

/***********************************************************************/
LSAbundance::LSAbundance() : DiversityCalculator(true) {}
/***********************************************************************/

vector<double> LSAbundance::getValues(int nMax, vector<mcmcSample>& sampling) { //int nMax = rank->getMaxRank();
    try {
        
        nMax = floor(pow(2.0,ceil(log((double) nMax)/log(2.0)) + 2.0) + 1.0e-7);
        
        results.resize(nMax, 0.0);
        int nSamples = sampling.size();
        
        if (nSamples == 0) {  return results; }
        
#ifdef USE_GSL
        
        DiversityUtils dutils("lsabund");
        
        gsl_set_error_handler_off();
        
        for(int i = 0; i < sampling.size(); i++) {
            
            if (m->getControl_pressed()) { break; }
            
            for (int j = 1; j <= nMax; j++) {
                int nA = j;
                double dLog = 0.0, dP = 0.0;
                
                if(nA < 100){ //MAX_QUAD
                    dLog = dutils.logLikelihoodQuad(nA, sampling[i].alpha, sampling[i].beta, sampling[i].dNu);
                }
                else{
                    dLog = dutils.logLikelihoodRampal(nA, sampling[i].alpha, sampling[i].beta, sampling[i].dNu);
                }
                
                dP = exp(dLog);
                
                results[j - 1] += dP*sampling[i].ns;
            }
            
        }
        
        for (int i = 1; i<=nMax; i++) {
            results[i-1] /= (double)nSamples;
            
            if (isnan(results[i-1]) || isinf(results[i-1])) { results[i-1] = 0.0; }
        }
        
#endif
        
        return results;
    }
    catch(exception& e) {
        m->errorOut(e, "LSAbundance", "getValues");
        exit(1);
    }
}
/***********************************************************************/

