/*
 *$Id: mod_fix.c 7905 2011-04-25 14:01:15Z bogdan_iancu $
 *
 * Copyright (C) 2001-2003 FhG Fokus
 *
 * This file is part of opensips, a free SIP server.
 *
 * opensips is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version
 *
 * opensips is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*!
 * \file
 * \brief Generic fixup functions for module function parameter.
 * - \ref FixupNameFormat
 */

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#include "mem/mem.h"
#include "str.h"
#include "ut.h"
#include "error.h"
#include "pvar.h"
#include "mod_fix.h"

/*!
 * \page FixupNameFormat Fixup Naming format
 * NAMING FORMAT
 * === fixup functions ===
 * + fixup_type1_type2(...)
 * - type1 - is the type the fist parameter gets converted to
 * - type2 - is the type the second parameter gets converted to
 * + if the parameter is missing, then use 'null'
 * + if the parameter is not converted, then use 'none'
 *
 * === fixup free functions ===
 * + free_fixup_type1_type2(...)
 * - type1 and type2 are same as for fixup function
 *
 * === helper functions ===
 * + functions to be used internaly for fixup/free functions
 * + fixup_type(...)
 * + fixup_free_type(...)
 * - type - is the type of the parameter that gets converted to/freed
 */


/*! \brief
 * helper function
 * Convert char* parameter to str* parameter
 */
int fixup_str(void** param)
{
	str* s;
	
	s = (str*)pkg_malloc(sizeof(str));
	if (!s) {
		LM_ERR("no more pkg memory\n");
		return E_UNSPEC;
	}
		
	s->s = (char*)*param;
	s->len = strlen(s->s);
	*param = (void*)s;
	
	return 0;
}

/*! \brief
 * - helper function
 * free the str* parameter
 */
int fixup_free_str(void** param)
{
	if(*param) {
		pkg_free(*param);
		*param = 0;
	}
	return 0;
}

/*! \brief
 * fixup for functions that get one parameter
 * - first parameter is converted to str*
 */
int fixup_str_null(void** param, int param_no)
{
	if(param_no != 1)
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_str(param);
}

/*! \brief
 * fixup for functions that get two parameters
 * - first parameter is converted to str*
 * - second parameter is converted to str*
 */
int fixup_str_str(void** param, int param_no)
{
	if (param_no != 1 && param_no != 2 )
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_str(param);
}

/*! \brief
 * fixup free for functions that get one parameter
 * - first parameter was converted to str*
 */
int fixup_free_str_null(void** param, int param_no)
{
	if(param_no != 1)
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_free_str(param);
}

/*! \brief
 * fixup free for functions that get two parameters
 * - first parameter was converted to str*
 * - second parameter was converted to str*
 */
int fixup_free_str_str(void** param, int param_no)
{
	if (param_no != 1 && param_no != 2 )
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_free_str(param);
}

/*! \brief
 * - helper function
 * Convert char* parameter to unsigned int
 * - the input parameter must be pkg-allocated and will be freed by function
 *   (it is how it comes from the config parser)
 */
int fixup_uint(void** param)
{
	unsigned int ui;
	str s;

	s.s = (char*)*param;
	s.len = strlen(s.s);
	if(str2int(&s, &ui)==0)
	{
		pkg_free(*param);
		*param=(void *)(unsigned long)ui;
		return 0;
	}
	LM_ERR("bad number <%s>\n", (char *)(*param));
	return E_CFG;
}

/*! \brief
 * fixup for functions that get one parameter
 * - first parameter is converted to unsigned int
 */
int fixup_uint_null(void** param, int param_no)
{
	if(param_no != 1)
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_uint(param);
}

/*! \brief
 * fixup for functions that get two parameters
 * - first parameter is converted to unsigned int
 * - second parameter is converted to unsigned int
 */
int fixup_uint_uint(void** param, int param_no)
{
	if (param_no != 1 && param_no != 2 )
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_uint(param);
}

#if 0
/*! \brief
 * - helper function
 * Convert char* parameter to signed int
 * - the input parameter must be pkg-allocated and will be freed by function
 *   (it is how it comes from the config parser)
 */
int fixup_sint( void** param)
{
	int si;
	str s;

	s.s = (char*)*param;
	s.len = strlen(s.s);
	if(str2sint(&s, &si)==0)
	{
		pkg_free(*param);
		*param=(void *)si;
		return 0;
	}
	LM_ERR("bad number <%s>\n", (char *)(*param));
	return E_CFG;
}

/*! \brief
 * fixup for functions that get one parameter
 * - first parameter is converted to signed int
 */
int fixup_sint_null(void** param, int param_no)
{
	if(param_no != 1)
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_sint(param);
}

/*! \brief
 * fixup for functions that get two parameters
 * - first parameter is converted to signed int
 * - second parameter is converted to signed int
 */
int fixup_sint_sint(void** param, int param_no)
{
	if (param_no != 1 && param_no != 2 )
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_sint(param);
}

/*! \brief
 * fixup for functions that get two parameters
 * - first parameter is converted to signed int
 * - second parameter is converted to unsigned int
 */
int fixup_sint_uint(void** param, int param_no)
{
	if (param_no != 1 && param_no != 2 )
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	if (param_no == 1)
		return fixup_sint(param);
	return fixup_uint(param);
}

/*! \brief
 * fixup for functions that get two parameters
 * - first parameter is converted to unsigned int
 * - second parameter is converted to signed int
 */
int fixup_uint_sint(void** param, int param_no)
{
	if (param_no != 1 && param_no != 2 )
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	if (param_no == 1)
		return fixup_uint(param);
	return fixup_sint(param);
}
#endif

/*! \brief
 * - helper function: Convert char* parameter to regular expression structure
 * - the input parameter must be pkg-allocated and will be freed by function
 *   (it is how it comes from the config parser)
 */
static int fixup_regexp(void** param, int rflags)
{
	regex_t* re;

	if ((re=pkg_malloc(sizeof(regex_t)))==0) {
		LM_ERR("no more pkg memory\n");
		return E_OUT_OF_MEM;
	}
	if (regcomp(re, *param, (REG_EXTENDED|REG_ICASE|REG_NEWLINE)&(~rflags))) {
		pkg_free(re);
		LM_ERR("bad re %s\n", (char*)*param);
		return E_BAD_RE;
	}
	/* free string */
	pkg_free(*param);
	/* replace it with the compiled re */
	*param=re;
	return 0;
}

/*! \brief
 * - helper function: free the regular expression parameter
 */
int fixup_free_regexp(void** param)
{
	if(*param)
	{
		regfree((regex_t*)(*param));
		pkg_free(*param);
		*param = 0;
	}
	return 0;
}

/*! \brief
 * fixup for functions that get one parameter
 * - first parameter is converted to regular expression structure
 */
int fixup_regexp_null(void** param, int param_no)
{
	if(param_no != 1)
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_regexp(param, 0);
}

/*! \brief
 * fixup for functions that get one parameter
 * - first parameter is converted to regular expression structure
 *   where "match-any-character" operators also match a newline
 */
int fixup_regexpNL_null(void** param, int param_no)
{
	if(param_no != 1)
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_regexp(param, REG_NEWLINE);
}

/**
 * fixup free for functions that get one parameter
 * - first parameter was converted to regular expression
 */
int fixup_free_regexp_null(void** param, int param_no)
{
	if(param_no != 1)
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_free_regexp(param);
}

/*! \brief
 * fixup for functions that get two parameters
 * - first parameter is converted to regular expression structure
 * - second parameter is not converted
 */
int fixup_regexp_none(void** param, int param_no)
{
	if (param_no != 1 && param_no != 2 )
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	if (param_no == 1)
		return fixup_regexp(param, 0);
	return 0;
}

/*! \brief
 * fixup for functions that get two parameters
 * - first parameter is converted to regular expression structure
 *   where "match-any-character" operators also match a newline
 * - second parameter is not converted
 */
int fixup_regexpNL_none(void** param, int param_no)
{
	if (param_no != 1 && param_no != 2 )
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	if (param_no == 1)
		return fixup_regexp(param, REG_NEWLINE);
	return 0;
}

/**
 * fixup free for functions that get two parameters
 * - first parameter was converted to regular expression
 * - second parameter was notconverted
 */
int fixup_free_regexp_none(void** param, int param_no)
{
	if (param_no != 1 && param_no != 2 )
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	if (param_no == 1)
		return fixup_free_regexp(param);
	return 0;
}


/*! \brief
 * - helper function: Convert char* parameter to PV spec structure
 */
int fixup_pvar(void **param)
{
	pv_spec_t *sp;
	str s;

	sp = (pv_spec_t*)pkg_malloc(sizeof(pv_spec_t));
	if (sp == 0) {
		LM_ERR("no pkg memory left\n");
		return E_UNSPEC;
	}
	s.s = (char*)*param; s.len = strlen(s.s);
	if (pv_parse_spec(&s, sp) == 0) {
		LM_ERR("parsing of pseudo variable %s failed!\n", (char*)*param);
		pkg_free(sp);
		return E_UNSPEC;
	}
	if (sp->type == PVT_NULL) {
		LM_ERR("bad pseudo variable\n");
		pkg_free(sp);
		return E_UNSPEC;
	}
	*param = (void*)sp;

	return 0;
}

/*! \brief
 * - helper function: free the PV parameter
 */
int fixup_free_pvar(void** param)
{
    if (*param) {
		pv_spec_free((pv_spec_t*)*param);
    }

    return 0;
}

/*! \brief
 * fixup for functions that get one parameter
 * - first parameter is converted to PV spec
 */
int fixup_pvar_null(void** param, int param_no)
{
	if(param_no != 1)
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_pvar(param);
}

/*! \brief
 * fixup free for functions that get one parameter
 * - first parameter was converted to PV spec
 */
int fixup_free_pvar_null(void** param, int param_no)
{
	if(param_no != 1)
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_free_pvar(param);
}

/*! \brief
 * fixup for functions that get two parameters
 * - both parameters are converted to PV spec
 */
int fixup_pvar_pvar(void** param, int param_no)
{
	if (param_no == 1)
	{
	    return fixup_pvar(param);
	}
	if (param_no != 2)
	{
	    LM_ERR("invalid parameter number %d\n", param_no);
	    return E_UNSPEC;
	}
	return fixup_pvar(param);
}

/*! \brief
 * fixup free for functions that get two parameters
 * - both parameters were converted to PV spec
 */
int fixup_free_pvar_pvar(void** param, int param_no)
{
	if(param_no == 1)
	{
	    return fixup_free_pvar(param);
	}
	if (param_no != 2)
	{
	    LM_ERR("invalid parameter number %d\n", param_no);
	    return E_UNSPEC;
	}	
	return fixup_free_pvar(param);
}

/*! \brief
 * fixup for functions that get two parameters
 * - first parameter is converted to PV spec
 * - second parameter is converted to str*
 */
int fixup_pvar_str(void** param, int param_no)
{
	if (param_no == 1)
	{
	    return fixup_pvar(param);
	}
	if (param_no != 2)
	{
	    LM_ERR("invalid parameter number %d\n", param_no);
	    return E_UNSPEC;
	}
	return fixup_str(param);
}

/*! \brief
 * fixup free for functions that get two parameters
 * - first parameter was converted to PV spec
 * - second parameter was converted to str*
 */
int fixup_free_pvar_str(void** param, int param_no)
{
	if(param_no == 1)
	{
	    return fixup_free_pvar(param);
	}
	if (param_no != 2)
	{
	    LM_ERR("invalid parameter number %d\n", param_no);
	    return E_UNSPEC;
	}
	return fixup_free_str(param);
}

/*! \brief
 * fixup for functions that get three parameters
 * - first parameter is converted to PV spec
 * - second parameter is converted to str*
 * - third parameter is converted to str*
 */
int fixup_pvar_str_str(void** param, int param_no)
{
	if (param_no == 1)
	{
	    return fixup_pvar(param);
	}
	if (param_no != 2 && param_no != 3)
	{
	    LM_ERR("invalid parameter number %d\n", param_no);
	    return E_UNSPEC;
	}
	return fixup_str(param);
}

/*! \brief
 * fixup free for functions that get three parameters
 * - first parameter was converted to PV spec
 * - second parameter was converted to str*
 * - third parameter was converted to str*
 */
int fixup_free_pvar_str_str(void** param, int param_no)
{
	if(param_no == 1)
	{
	    return fixup_free_pvar(param);
	}
	if (param_no != 2 && param_no != 3)
	{
	    LM_ERR("invalid parameter number %d\n", param_no);
	    return E_UNSPEC;
	}
	return fixup_free_str(param);
}

/*! \brief
 * - helper function
 * Convert char* parameter to gparam_t (int or PV)
 */
int fixup_igp(void** param)
{
	str s;
	gparam_p gp = NULL;
	
	gp = (gparam_p)pkg_malloc(sizeof(gparam_t));
	if(gp == NULL)
	{
		LM_ERR("no more memory\n");
		return E_UNSPEC;
	}
	memset(gp, 0, sizeof(gparam_t));
	s.s = (char*)*param; s.len = strlen(s.s);
	if(s.s[0]==PV_MARKER)
	{
		gp->type = GPARAM_TYPE_PVS;
		gp->v.pvs = (pv_spec_t*)pkg_malloc(sizeof(pv_spec_t));
		if (gp->v.pvs == NULL)
		{
			LM_ERR("no pkg memory left for pv_spec_t\n");
		    pkg_free(gp);
		    return E_UNSPEC;
		}

		if(pv_parse_spec(&s, gp->v.pvs)==NULL)
		{
			LM_ERR("Unsupported User Field identifier\n");
		    pkg_free(gp->v.pvs);
		    pkg_free(gp);
			return E_UNSPEC;
		}
	} else {
		gp->type = GPARAM_TYPE_INT;
		if(str2sint(&s, &gp->v.ival) != 0)
		{
			LM_ERR("Bad number <%s>\n", (char*)(*param));
			return E_UNSPEC;
		}
	}
	*param = (void*)gp;

	return 0;
}

int fixup_sgp(void** param)
{
	str s;
	gparam_p gp = NULL;
	
	gp = (gparam_p)pkg_malloc(sizeof(gparam_t));
	if(gp == NULL)
	{
		LM_ERR("no more memory\n");
		return E_UNSPEC;
	}
	memset(gp, 0, sizeof(gparam_t));
	s.s = (char*)*param; s.len = strlen(s.s);
	if(s.s[0]==PV_MARKER)
	{
		gp->type = GPARAM_TYPE_PVS;
		gp->v.pvs = (pv_spec_t*)pkg_malloc(sizeof(pv_spec_t));
		if (gp->v.pvs == NULL)
		{
			LM_ERR("no pkg memory left for pv_spec_t\n");
		    pkg_free(gp);
		    return E_UNSPEC;
		}

		if(pv_parse_spec(&s, gp->v.pvs)==NULL)
		{
			LM_ERR("Unsupported User Field identifier\n");
		    pkg_free(gp->v.pvs);
		    pkg_free(gp);
			return E_UNSPEC;
		}
	} else {
		gp->type = GPARAM_TYPE_STR;
		gp->v.sval = s;
	}
	*param = (void*)gp;

	return 0;
}
/*! \brief
 * fixup for functions that get one parameter
 * - first parameter is converted to gparam_t (int or PV)
 */
int fixup_igp_null(void** param, int param_no)
{
	if (param_no != 1)
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_igp(param);
}

int fixup_sgp_null(void** param, int param_no)
{
	if (param_no != 1)
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_sgp(param);
}

/*! \brief
 * fixup for functions that get two parameters
 * - first parameter is converted to gparam_t (int or PV)
 * - second parameter is converted to gparam_t (int or PV)
 */
int fixup_igp_igp(void** param, int param_no)
{
	if (param_no != 1 && param_no != 2 )
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_igp(param);
}

/*! \brief
 * fixup for functions that get three parameters
 * - first parameter is converted to gparam_t (int or PV)
 * - second parameter is converted to gparam_t (int or PV)
 * - third parameter is converted to gparam_t (int or PV)
 */
int fixup_igp_igp_igp(void** param, int param_no)
{
	if (param_no < 1 || param_no > 3 )
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_igp(param);
}

int fixup_sgp_sgp(void** param, int param_no)
{
	if (param_no != 1 && param_no != 2 )
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_sgp(param);
}
/*! \brief
 * fixup for functions that get three parameters
 * - first parameter is converted to gparam_t (int or PV)
 * - second parameter is converted to PV spec
 * - third parameter is converted to PV spec
 */
int fixup_igp_pvar_pvar(void** param, int param_no)
{
    if(param_no == 1) {
	return fixup_igp(param);
    }
    if (param_no != 2 && param_no != 3)	{
	LM_ERR("invalid parameter number %d\n", param_no);
	return E_UNSPEC;
    }
    return fixup_pvar(param);
}

/*! \brief
 * fixup free for functions that get three parameters
 * - first parameter was converted to gparam_t (int or PV)
 * - second parameter was converted to PV spec
 * - third parameter was converted to PV spec
 */
int fixup_free_igp_pvar_pvar(void** param, int param_no)
{
	if(param_no == 1) {
	    return 0;
	}
	if (param_no != 2 && param_no != 3) {
	    LM_ERR("invalid parameter number %d\n", param_no);
	    return E_UNSPEC;
	}
	return fixup_free_pvar(param);
}

/*! \brief
 * - helper function
 * Return integer value from a gparam_t
 */
int fixup_get_ivalue(struct sip_msg* msg, gparam_p gp, int *val)
{
	pv_value_t value;

	if(gp->type==GPARAM_TYPE_INT)
	{
		*val = gp->v.ival;
		return 0;
	}
	
	if(pv_get_spec_value(msg, gp->v.pvs, &value)!=0 
			|| value.flags&PV_VAL_NULL || !(value.flags&PV_VAL_INT))
	{
		LM_ERR("no valid PV value found (error in scripts)\n");
		return -1;
	}
	*val = value.ri;
	return 0;
}

/*! \brief
 * - helper function
 * Convert char* parameter to gparam_t (str or pv_elem_t)
 */
int fixup_spve(void** param)
{
	str s;
	gparam_p gp = NULL;
	
	gp = (gparam_p)pkg_malloc(sizeof(gparam_t));
	if(gp == NULL)
	{
		LM_ERR("no more memory\n");
		return E_UNSPEC;
	}
	memset(gp, 0, sizeof(gparam_t));

	s.s = (char*)(*param); s.len = strlen(s.s);
	if(pv_parse_format(&s, &gp->v.pve)<0)
	{
		LM_ERR("wrong format[%s]\n", s.s);
		return E_UNSPEC;
	}
	if(gp->v.pve->spec.getf==NULL)
	{
		gp->type = GPARAM_TYPE_STR;
		pv_elem_free_all(gp->v.pve);
		gp->v.sval = s;
	} else {
		gp->type = GPARAM_TYPE_PVE;
	}			
	*param = (void*)gp;
	return 0;
}

/*! \brief
 * fixup for functions that get one parameter
 * - first parameter is converted to gparam_t (str or pv_elem_t)
 */
int fixup_spve_null(void** param, int param_no)
{
	if (param_no != 1)
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_spve(param);
}

/*! \brief
 * fixup for functions that get two parameters
 * - first parameter is converted to gparam_t (str or pv_elem_t)
 * - second parameter is converted to gparam_t (str or pv_elem_t)
 */
int fixup_spve_spve(void** param, int param_no)
{
	if (param_no != 1 && param_no != 2 )
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	return fixup_spve(param);
}

/*! \brief
 * fixup for functions that get two parameters
 * - first parameter is converted to gparam_t (str or pv_elem_t)
 * - second parameter is converted to uint
 */
int fixup_spve_uint(void** param, int param_no)
{
	if (param_no != 1 && param_no != 2 )
	{
		LM_ERR("invalid parameter number %d\n", param_no);
		return E_UNSPEC;
	}
	if (param_no == 1)
		return fixup_spve(param);
		
	return fixup_uint(param);
}

/*! \brief
 * - helper function
 * Return string value from a gparam_t
 */
int fixup_get_svalue(struct sip_msg* msg, gparam_p gp, str *val)
{
	pv_value_t value;

	if(gp->type==GPARAM_TYPE_STR)
	{
		*val = gp->v.sval;
		return 0;
	}
	
	if(gp->type==GPARAM_TYPE_PVS)
	{
		if(pv_get_spec_value(msg, gp->v.pvs, &value)!=0 
				|| value.flags&PV_VAL_NULL || !(value.flags&PV_VAL_STR))
		{
			LM_ERR("no valid PV value found (error in scripts)\n");
			return -1;
		}
		*val = value.rs;
		return 0;
	}

	if(gp->type==GPARAM_TYPE_PVE)
	{
		if(pv_printf_s( msg, gp->v.pve, val)!=0)
		{
			LM_ERR("cannot print the PV-formated string\n");
			return -1;
		}
		return 0;
	}

	return -1;
}

