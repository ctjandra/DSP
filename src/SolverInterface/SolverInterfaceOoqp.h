/*
 * SolverInterfaceOoqp.h
 *
 *  Created on: Jan 7, 2015
 *      Author: kibaekkim
 */

#ifndef SRC_SOLVER_SOLVERINTERFACEOOQP_H_
#define SRC_SOLVER_SOLVERINTERFACEOOQP_H_

//#define DSP_HAS_MA57

/** DSP */
#include "SolverInterface/SolverInterface.h"
#include "Utility/StoMessage.h"

/** OOQP */
#include "QpGenData.h"
#include "QpGenVars.h"
#include "QpGenResiduals.h"
#include "GondzioSolver.h"
#include "MehrotraSolver.h"
#ifdef DSP_HAS_MA57
#include "QpGenSparseMa57.h"
#else
#include "QpGenSparseMa27.h"
#endif

class SolverInterfaceOoqp: public SolverInterface
{
public:
	SolverInterfaceOoqp(StoParam * par) :
		SolverInterface(par),
		sense_(1),
		status_(STO_STAT_UNKNOWN),
		print_level_(0),
		qp_(NULL),
		prob_(NULL),
		vars_(NULL),
		resid_(NULL),
		solver_(NULL),
		released_(true),
		my_(0),
		mz_(0),
		nrows_(0),
		ncols_(0),
		mat_(NULL),
		clbd_(NULL),
		cubd_(NULL),
		obj_(NULL),
		rlbd_(NULL),
		rubd_(NULL),
		objval_(0),
		x_(NULL),
		y_(NULL),
		lambda_(NULL),
		pi_(NULL),
		gamma_(NULL),
		phi_(NULL),
		nLambdas_(0),
		nPis_(0),
		nIters_(0) {}

	/** copy constructor */
	SolverInterfaceOoqp(SolverInterfaceOoqp * si);

	/** clone */
	virtual SolverInterface * clone();

	virtual ~SolverInterfaceOoqp();

	/** load problem */
	virtual void loadProblem(
			OsiSolverInterface * si,
			const char * probname = "null");

	/** load problem */
	virtual void loadProblem(
			CoinPackedMatrix * mat,
			const double * collb,
			const double * colub,
			const double * obj,
			const char * ctype,
			const double * rowlb,
			const double * rowub,
			const char * probname = "null");

	/** core part for load problem */
	virtual void gutsOfLoadProblem();

	/** add row */
	virtual void addRow(int size, const int * indices, const double * vals, double lb, double ub);

	/** add cuts */
	virtual int addCuts(OsiCuts cuts, double effectiveness = 0.0);

	/** solve */
	virtual void solve();

	/** get objective sense */
	virtual int getObjSense() {return sense_;}

	/** solution status */
	virtual STO_RTN_CODE getStatus() {return status_;}

	/** get number of rows */
	virtual int getNumRows() {return my_ + mz_;}

	/** get number of equality constraints */
	virtual int getNumEqConss() {return my_;}

	/** get number of inequality constraints */
	virtual int getNumIneqConss() {return mz_;}

	/** get number of columns */
	virtual int getNumCols() {return ncols_;}

	/** get constraint matrix */
	virtual CoinPackedMatrix * getMatrix() {return mat_;}

	/** get column lower bounds */
	virtual const double * getColLower() {return clbd_;}

	/** get column upper bounds */
	virtual const double * getColUpper() {return cubd_;}

	/** get objective */
	virtual const double * getObjective() {return obj_;}

	/** get row lower bounds */
	virtual const double * getRowLower() {return rlbd_;}

	/** get row upper bounds */
	virtual const double * getRowUpper() {return rubd_;}

	/** get global primal bound (upper bound in minimization) */
	virtual double getPrimalBound() {return objval_ * sense_;}

	/** get dual bound (lower bound in minimization) */
	virtual double getDualBound() {return objval_ * sense_;}

	/** get solution values */
	virtual const double * getSolution() {return x_;}

	/** get solution values */
	virtual const double * y() {return y_;}

	/** get number of lambdas */
	virtual int getNumLambdas() {return nLambdas_;}

	/** get number of pis */
	virtual int getNumPis() {return nPis_;}

	/** get lambda */
	virtual const double * lambda() {return lambda_;}

	/** get pi */
	virtual const double * pi() {return pi_;}

	/** get gamma */
	virtual const double * gamma() {return gamma_;}

	/** get phi */
	virtual const double * phi() {return phi_;}

	/** get number of iterations */
	virtual int getIterationCount() {return nIters_;}

	/** get number of nodes */
	virtual int getNumNodes() {return 0;}

	/** get print out level */
	virtual int getPrintLevel() {return print_level_;}

	/** OOQP objects release? */
	virtual bool released() {return released_;}

	/** set objective coefficients */
	virtual void setObjSense(int sense) {sense_ = sense;}

	/** set objective coefficients */
	virtual void setObjCoef(double * obj);

	/** set column bounds */
	virtual void setColLower(int index, double lb);

	/** set column bounds */
	virtual void setColUpper(int index, double ub);

	/** set column bounds */
	virtual void setColBounds(int index, double lb, double ub);

	/** set row bounds */
	virtual void setRowLower(int index, double lb);

	/** set row bounds */
	virtual void setRowUpper(int index, double ub);

	/** set row bounds */
	virtual void setRowBounds(int index, double lb, double ub);

	/** set node limit */
	virtual void setNodeLimit(int limit) {}

	/** set iteration limit */
	virtual void setIterLimit(int limit) {}

	/** set wall time limit */
	virtual void setTimeLimit(double sec) {DSPdebugMessage("Not available for OOQP.\n");}

	/** set print out level */
	virtual void setPrintLevel(int level) {print_level_ = level;}

	/** write MPS file */
	virtual void writeMps(const char * filename) {DSPdebugMessage("Not available for OOQP.\n");}

protected:

	/** initialize solver interface */
	virtual STO_RTN_CODE initialize() {return STO_RTN_OK;}

	/** finalize solver interface */
	virtual STO_RTN_CODE finalize();

	/** release OOQP objects */
	virtual void releaseOOQP();

	/** release solver */
	void release();

public:

	OsiCuts cuts_; /**< for additional rows/columns */

protected:

	int sense_; /**< objective sense */
	STO_RTN_CODE status_; /**< solution status */
	int print_level_;

	/** OOQP objects */
	QpGenSparseSeq * qp_;
	QpGenData * prob_;
	QpGenVars * vars_;
	QpGenResiduals * resid_;
	Solver * solver_;
	bool released_;

	/** original data */
	int my_;
	int mz_;
	int nrows_;
	int ncols_;
	CoinPackedMatrix * mat_;
	double * clbd_;
	double * cubd_;
	double * obj_;
	double * rlbd_;
	double * rubd_;

	double objval_;   /**< objective value */
	double * x_;      /**< primal solution */
	double * y_;      /**< dual variables corresponding to equality constraints */
	double * lambda_; /**< dual variables corresponding to inequality (>=) constraints */
	double * pi_;     /**< dual variables corresponding to inequality (<=) constraints */
	double * gamma_;  /**< dual variables corresponding to column lower bounds */
	double * phi_;    /**< dual variables corresponding to column upper bounds */
	int nLambdas_;    /**< number of lambdas */
	int nPis_;        /**< number of pis */
	int nIters_;      /**< number of iterations */
};

#endif /* SRC_SOLVER_SOLVERINTERFACEOOQP_H_ */
