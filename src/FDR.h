#ifndef FDR_H_
#define FDR_H_

#include "BackgroundModel.h"
#include "utils.h"
#include "Motif.h"
#include "ModelLearning.h"
#include "SeqGenerator.h"
#include "ScoreSeqSet.h"

class FDR {

	/*
	 * FDR class deals with N-fold cross-validation:
	 * 1. calculate log odds scores for each position on each sequence;
	 * 2. generate negative sequence set based on k-mer frequencies if
	 *    no negative sequence set is given;
	 * 3. calculate true positives(TP), false positives(FP), false
	 *    discovery rate(FDR) and recall values;
	 * 4. calculate p-values due to log odds scores, in order to use
	 *    fdrtool R package for further estimation.
	 */

public:

	FDR( std::vector<Sequence*> posSeqs,
		std::vector<Sequence*> negSeqs,
		float q = 0.9f,
		Motif* motif = NULL,
		size_t cvFold = 5 );
	~FDR();

	void 	evaluateMotif();
	void 	print();
	void	write( char* odir, std::string basename, size_t n );

private:

	std::vector<Sequence*> posSeqs_;
	std::vector<Sequence*> negSeqs_;
	float q_;

	Motif*				motif_;			// initial motif

	size_t				cvFold_;		// fold for cross-validation, training
										// set is (cv-1)-fold of the testing set
	bool				mops_			= Global::mops;
	bool				zoops_			= Global::zoops;
	bool				EM_ 			= Global::EM;
	bool				CGS_ 			= Global::CGS;
	bool				savePRs_ 		= Global::savePRs;
	bool				savePvalues_	= Global::savePvalues;
	bool				saveLogOdds_	= Global::saveLogOdds;

	std::vector<float> 	posScoreAll_;	// store log odds scores over all positions on the sequences
	std::vector<float> 	posScoreMax_;	// store maximal log odds score from each sequence
	std::vector<float> 	negScoreAll_;
	std::vector<float> 	negScoreMax_;

	std::vector<float>	ZOOPS_FDR_;		// precision for ZOOPS model
	std::vector<float>	ZOOPS_Rec_;		// recall for ZOOPS model
	std::vector<float>  ZOOPS_TP_;		// true positives for ZOOPS model
	std::vector<float>  ZOOPS_FP_;		// false positives for ZOOPS model

	std::vector<float>	MOPS_FDR_;		// precision for MOPS model
	std::vector<float>	MOPS_Rec_;		// recall for MOPS model
	std::vector<float>  MOPS_TP_;		// true positives for MOPS model
	std::vector<float>  MOPS_FP_;		// false positives for MOPS model

	float				occ_frac_;		// fraction of motif occurrence
	float				occ_mult_;		// number of motif occurrences per sequence

	std::vector<float>	PN_Pvalue_;		// p-values for scores from both positive
										// and negative set with ZOOPS model used
										// for benchmarking
	std::vector<float>	ZOOPS_Pvalue_;	// p-values for scores from positive set with ZOOPS model
	std::vector<float>	MOPS_Pvalue_;	// p-values for scores from positive set with MOPS model

			// calculate precision and recall for both ZOOPS and MOPS models
	void 	calculatePR();

			// calculate P-values for log odds scores of positive sequences
	void	calculatePvalues();

};

#endif /* FDR_H_ */
