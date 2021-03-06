/*
 * SCIPconshdlrBenders.h
 *
 *  Created on: Dec 8, 2014
 *      Author: kibaekkim
 */

#ifndef SCIPCONSHDLRBENDERS_H_
#define SCIPCONSHDLRBENDERS_H_

/** SCIP */
#include "objscip/objconshdlr.h"

/** DSP */
#include "Solver/TssBdSub.h"

class SCIPconshdlrBenders : public scip::ObjConshdlr
{
public:

	enum whereFrom
	{
		from_scip_sepalp = 0,
		from_scip_sepasol,
		from_scip_enfolp,
		from_scip_enfops,
		from_scip_check
	};

	/** default constructor */
	SCIPconshdlrBenders(SCIP * scip, int sepapriority) :
		ObjConshdlr(scip, "Benders", "Benders cuts",
				sepapriority, /**< priority of the constraint handler for separation */
				sepapriority, /**< priority of the constraint handler for constraint enforcing */
				sepapriority, /**< priority of the constraint handler for checking infeasibility (and propagation) */
				1,        /**< always call separator */
				-1,       /**< disable constraint propagation */
				1,        /**< always use all constraints (no aging) */
				0,        /**< disable the preprocessing callback of the constraint handler */
				TRUE,     /**< delay separation method */
				FALSE,    /**< do not delay propatation method */
				FALSE,    /**< do not delay presolving method */
				TRUE,     /**< skip constraint handler even if no constraints are available. */
				SCIP_PROPTIMING_BEFORELP /**< propagation method is called before solving LP */),
		tss_(NULL), nvars_(0), vars_(NULL), naux_(0)
	{
		/** nothing to do */
	}

	/** default constructor */
	virtual ~SCIPconshdlrBenders()
	{
		/** nothing to do */
	}

	/** destructor of constraint handler to free user data (called when SCIP is exiting) */
	virtual SCIP_DECL_CONSFREE(scip_free);

	/** transforms constraint data into data belonging to the transformed problem */
	virtual SCIP_DECL_CONSTRANS(scip_trans);

	/** separation method of constraint handler for LP solution */
	virtual SCIP_DECL_CONSSEPALP(scip_sepalp);

	/** separation method of constraint handler for arbitrary primal solution */
	virtual SCIP_DECL_CONSSEPASOL(scip_sepasol);

	/** constraint enforcing method of constraint handler for LP solutions */
	virtual SCIP_DECL_CONSENFOLP(scip_enfolp);

	/** constraint enforcing method of constraint handler for pseudo solutions */
	virtual SCIP_DECL_CONSENFOPS(scip_enfops);

	/** feasibility check method of constraint handler for primal solutions */
	virtual SCIP_DECL_CONSCHECK(scip_check);

	/** variable rounding lock method of constraint handler */
	virtual SCIP_DECL_CONSLOCK(scip_lock);

	/** returns whether the objective plugin is copyable */
	virtual SCIP_DECL_CONSHDLRISCLONEABLE(iscloneable) {return true;}

	/** clone method which will be used to copy constraint handler and variable pricer objects */
	virtual SCIP_DECL_CONSHDLRCLONE(ObjProbCloneable* clone);

public:

	/** assign pointer to cut generator */
	void assignTssBdSub(TssBdSub * tss)
	{
		tss_ = tss;
		naux_ = tss->nAuxvars_;
	}

	/** set original variable pointers */
	SCIP_RETCODE setOriginalVariables(int nvars, SCIP_Var ** vars);

private:

	SCIP_RETCODE sepaBenders(
			SCIP * scip,
			SCIP_CONSHDLR * conshdlr,
			SCIP_SOL * sol,
			whereFrom where,
			SCIP_RESULT * result);

private:

	/** TODO: These could go in SCIP_ConsData. Does it matter? */
	TssBdSub * tss_;   /**< pointer to cut generator */
	int nvars_;        /**< number of original variables */
	SCIP_Var ** vars_; /**< pointer array to original variables */
	int naux_;         /**< number of auxiliary variables */
};

/** creates and captures a Benders constraint */
SCIP_RETCODE SCIPcreateConsBenders(
		SCIP * scip,
		SCIP_CONS ** cons,
		const char * name);

#endif /* SCIPCONSHDLRBENDERS_H_ */
