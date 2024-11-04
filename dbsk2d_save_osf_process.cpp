//
// Created by wenhan on 9/4/17.
//

#include <cstdio>
#include "dbsk2d_save_osf_process.h"
#include "dbsk2d/dbsk2d_ishock_pointline.h"
#include "dbsk2d/dbsk2d_ishock_pointpoint.h"
#include "dbsk2d/dbsk2d_ishock_lineline.h"
#include "dbsk2d/dbsk2d_ishock_contact.h"

#define DOUBLE_PRECISION 20

dbsk2d_save_osf_process::dbsk2d_save_osf_process() : bpro1_process()
{
    _boundary = NULL;
    _ishock_graph = NULL;

    if( !parameters()->add( "Output .osf file <file_name>", "-osfoutput", bpro1_filepath("output_shock.osf", "*osf")))
        vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;

}

bpro1_process *dbsk2d_save_osf_process::clone() const
{
    return new dbsk2d_save_osf_process(*this);
}

vcl_vector< vcl_string > dbsk2d_save_osf_process::get_input_type()
{
    vcl_vector< vcl_string > to_return;
    to_return.clear();
    return to_return;
}

vcl_vector< vcl_string > dbsk2d_save_osf_process::get_output_type()
{
    vcl_vector< vcl_string > to_return;
    to_return.push_back("osf");
    return to_return;
}

bool dbsk2d_save_osf_process::execute()
{
    bpro1_filepath output_path;

    if(!_boundary)
    {
        vcl_cout << "ERROR: empty boundary in save .osf!" << vcl_endl;
        return false;
    }
    if(!_ishock_graph)
    {
        vcl_cout << "ERROR: empty ishock_graph in save .osf!" << vcl_endl;
        return false;
    }

    parameters()->get_value("-osfoutput", output_path);
    vcl_string output_file = output_path.path;

    return save_osf(output_file);
}

bool dbsk2d_save_osf_process::save_osf(vcl_string file_name)
{
    vcl_ofstream outfp(file_name.c_str(), vcl_ios::out);

    outfp.setf(vcl_ios::fixed);

    if (!outfp){
        vcl_cerr << " Error opening file  " << file_name.c_str() << vcl_endl;
        return false;
    }

    outfp << "# ==============ISHOCK FILE==============" << vcl_endl;

    outfp << "# ==============BOUNDARY POINT=================" << vcl_endl;
    outfp << "# Boundary points:" << vcl_endl;
    outfp << "# [ID] [TYPE] [VISIBILITY] [MAX_ETA] [X] [Y] [VREF] [DIR] [CONF] [BGUI] [BND_VERTEX_ID]" << vcl_endl;
    outfp << "[BOUNDARY_POINT_BEGIN]" << vcl_endl;
    _boundary->update_belm_list();
    vcl_vector< dbsk2d_ishock_belm* > belms = _boundary->belm_list();
    for (vcl_vector< dbsk2d_ishock_belm* >::iterator it = belms.begin(); it != belms.end(); it++)
    {
        if ((*it)->type() == 0)
        {
            outfp << (*it)->id() << " ";
            outfp << (*it)->type() << " ";
	        outfp << ((dbsk2d_ishock_bpoint*)(*it))->is_visible() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << (*it)->max_eta() << " ";
            vgl_point_2d<double> pt = ((dbsk2d_ishock_bpoint*)(*it))->pt();
            outfp << vcl_setprecision(DOUBLE_PRECISION) << pt.x() << " " << pt.y() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << ((dbsk2d_ishock_bpoint*)(*it))->vref() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << ((dbsk2d_ishock_bpoint*)(*it))->tangent() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << ((dbsk2d_ishock_bpoint*)(*it))->conf() << " ";
	        outfp << (*it)->is_a_GUIelm() << " ";
	        outfp << ((dbsk2d_ishock_bpoint*)(*it))->bnd_vertex()->get_id() << " ";
            outfp << vcl_endl;
        }
    }

    outfp << "# ==============BOUNDARY LINE=================" << vcl_endl;
    outfp << "# Boundary lines:" << vcl_endl;
    outfp << "# [ID] [TYPE] [START_POINT] [END_POINT] [U] [N] [L] [BND_CONTOUR_ID] [SIZE] [BND_EDGE_ID] [LEFT_BCURVE_ID] [RIGHT_BCURVE_ID] [BND_V1_ID] [BND_BPOINT_ID] [BND_V2_VERTEX_ID] [BND_V2_BPOINT_ID]" << vcl_endl;
    outfp << "[BOUNDARY_LINE_BEGIN]" << vcl_endl;
    for (vcl_vector< dbsk2d_ishock_belm* >::iterator it = belms.begin(); it != belms.end(); it++)
    {
	    int a;
        if ((*it)->type() == 1)
        {
            outfp << (*it)->id() << " ";
            outfp << (*it)->type() << " ";
            dbsk2d_ishock_bline* line = (dbsk2d_ishock_bline*)(*it);
            outfp << line->s_pt()->id() << " ";
            outfp << line->e_pt()->id() << " ";
            outfp << vcl_setprecision(DOUBLE_PRECISION) << line->u() << " ";
            outfp << vcl_setprecision(DOUBLE_PRECISION) << line->n() << " ";
            outfp << vcl_setprecision(DOUBLE_PRECISION) << line->l() << " ";

	        dbsk2d_bnd_edge* edge = line->bnd_edge();

            // dbsk2d_bnd_contours
            outfp << line->get_contour_id() << " ";
	        const vcl_list< vtol_topology_object * > *
			        superiors  = edge->superiors_list();
	        vcl_list<vtol_topology_object*>::const_iterator tit;
	        for ( tit=(*superiors).begin(); tit!= (*superiors).end(); ++tit)
	        {
		        vcl_vector< signed char >* arr;
		        dbsk2d_bnd_contour* bnd_contour = static_cast<dbsk2d_bnd_contour*> (*tit);
		        arr = bnd_contour->directions();
		        outfp << arr->size() << " ";
	        }

            //dbsk2d_bnd_edge
            outfp << edge->get_id() << " ";
            outfp << edge->left_bcurve()->id() << " " << edge->right_bcurve()->id() << " ";
            outfp << edge->bnd_v1()->get_id() << " " << edge->bnd_v1()->bpoint()->id() << " ";
            outfp << edge->bnd_v2()->get_id() << " " << edge->bnd_v2()->bpoint()->id();
            outfp << vcl_endl;
        }
    }


	outfp << "# ==============ISHOCK NODE==============" << vcl_endl;
	outfp << "# IShock Nodes:" << vcl_endl;
	outfp << "# [ID] [x] [y] [START_TIME] [END_TIME] [CHILD_SHOCK_LINK_ID_1] [CHILD_SHOCK_LINK_ID_2] [ACTIVE] [PROPAGATED] [VALID] [HIDDEN] [ORIGIN_X] [ORIGIN_Y] [NUM_BND] [BND_1_ID] [BND_2_ID] ..." << vcl_endl;
	outfp << "[ISHOCK_NODE_BEGIN]" << vcl_endl;
	ishock_node_list nodes = _ishock_graph->all_nodes();
	for (ishock_node_list_iter inl = nodes.begin(); inl != nodes.end() ; inl++)
	{
		outfp << (*inl)->id() << " ";
		outfp << vcl_setprecision(DOUBLE_PRECISION) << (*inl)->origin().x() << " " << (*inl)->origin().y() << " ";
		outfp << vcl_setprecision(DOUBLE_PRECISION) << (*inl)->startTime() << " ";
		outfp << vcl_setprecision(DOUBLE_PRECISION) << (*inl)->endTime() << " ";
		if ((*inl)->cShock())
		{
			outfp << (*inl)->cShock()->id() << " ";
		}
		else
		{
			outfp << "-1 ";
		}
		if ((*inl)->cShock2())
		{
			outfp << (*inl)->cShock2()->id() << " ";
		}
		else
		{
			outfp << "-1 ";
		}

		//ishock_elm parameters
		outfp << (*inl)->isActive() << " " << (*inl)->hasPropagated() << " " << (*inl)->isValid() << " " << (*inl)->isHidden() << " ";
		outfp << vcl_setprecision(DOUBLE_PRECISION) << (*inl)->origin().x() << " " << (*inl)->origin().y() << " ";

		ishock_node_belm_list bnds = (*inl)->bndList();
		outfp << (*inl)->bndList().size() << " ";
		for (ishock_node_belm_list_iter inbl = bnds.begin(); inbl != bnds.end(); inbl++)
		{
			outfp << (*inbl).second->id() << " ";
		}
		outfp << vcl_endl;
	}

    outfp << "# ==============ISHOCK EDGE==============" << vcl_endl;
    outfp << "# IShock Edges:" << vcl_endl;
    outfp << "# [ID] [TYPE] [ACTIVE] [START_TIME] [SIM_TIME] [END_TIME] [LS_ETA] [RS_ETA] [LS_TAU] [LE_TAU] [RS_TAU] [RE_TAU] [H] [MIN_LTAU] [MAX_LTAU] [MIN_RTAU] [MAX_RTAU] [FROM_NODE_ID] [TO_NODE_ID] [L_SHOCK] [R_SHOCK] [L_NB] [R_NB] [LEFT_BND_ID] [RIGHT_BND_ID] [CELL_BND_TYPE] [CELL_BND_LOC] [INTERSECT_POS] [PROPAGATED] [VALID] [HIDDEN] [ORIGIN_X] [ORIGIN_Y] [OTHER_INFO]" << vcl_endl;
    outfp << "# [OTHER_INFO]: " << vcl_endl;
    outfp << "# for point-point: [N] [U]" << vcl_endl;
    outfp << "# for point-line: [NU] [U] [N] [L_DELTA] [R_DELTA] [L]" << vcl_endl;
    outfp << "# for line-line: [UR] [PHI] [UL] [SIGMA] [THETA_L] [THETA_R] [LL] [LR] [N1L] [N1R] [N2L] [N2R] [AL_X] [AL_Y] [BL_X] [BL_Y] [AR_X] [AR_Y] [BR_X] [BR_Y]" << vcl_endl;
    outfp << "# for contact: [N] [ORIGIN_X] [ORIGIN_Y] " << vcl_endl;
    outfp << "[ISHOCK_EDGE_BEGIN]" << vcl_endl;
    ishock_edge_list edges = _ishock_graph->all_edges();
    for (ishock_edge_list_iter iel = edges.begin(); iel != edges.end(); iel++)
    {
	    outfp << (*iel)->id() << " ";
	    outfp << (*iel)->type() << " ";
	    outfp << (*iel)->isActive() << " ";
	    outfp << vcl_setprecision(DOUBLE_PRECISION) << (*iel)->startTime() << " ";
	    outfp << vcl_setprecision(DOUBLE_PRECISION) << (*iel)->simTime() << " ";
	    outfp << vcl_setprecision(DOUBLE_PRECISION) << (*iel)->endTime() << " ";
	    outfp << vcl_setprecision(DOUBLE_PRECISION) << (*iel)->LsEta() << " ";
	    outfp << vcl_setprecision(DOUBLE_PRECISION) << (*iel)->RsEta() << " ";
	    outfp << vcl_setprecision(DOUBLE_PRECISION) << (*iel)->LsTau() << " ";
	    outfp << vcl_setprecision(DOUBLE_PRECISION) << (*iel)->LeTau() << " ";
	    outfp << vcl_setprecision(DOUBLE_PRECISION) << (*iel)->RsTau() << " ";
	    outfp << vcl_setprecision(DOUBLE_PRECISION) << (*iel)->ReTau() << " ";
	    outfp << vcl_setprecision(DOUBLE_PRECISION) << (*iel)->H() << " ";
	    outfp << vcl_setprecision(DOUBLE_PRECISION) << (*iel)->minLTau() << " ";
	    outfp << vcl_setprecision(DOUBLE_PRECISION) << (*iel)->maxLTau() << " ";
	    outfp << vcl_setprecision(DOUBLE_PRECISION) << (*iel)->minRTau() << " ";
	    outfp << vcl_setprecision(DOUBLE_PRECISION) << (*iel)->maxRTau() << " ";

	    if ((*iel)->pSNode())
		    outfp << (*iel)->pSNode()->id() << " ";
	    else
		    outfp << "-1 ";
	    if ((*iel)->cSNode())
		    outfp << (*iel)->cSNode()->id() << " ";
	    else
		    outfp << "-1 ";

	    if ((*iel)->lShock())
		    outfp << (*iel)->lShock()->id() << " ";
	    else
		    outfp << "-1 ";
	    if ((*iel)->rShock())
		    outfp << (*iel)->rShock()->id() << " ";
	    else
		    outfp << "-1 ";

	    if ((*iel)->lNeighbor())
		    outfp << (*iel)->lNeighbor()->id() << " ";
	    else
		    outfp << "-1 ";
	    if ((*iel)->rNeighbor())
		    outfp << (*iel)->rNeighbor()->id() << " ";
	    else
		    outfp << "-1 ";

	    outfp << (*iel)->lBElement()->id() << " " << (*iel)->rBElement()->id() << " ";


	    if((*iel)->cell_bnd())
	    {
		    dbsk2d_lagrangian_cell_bnd_sptr cell_bnd = (*iel)->cell_bnd();
		    outfp << cell_bnd->type() << " " ;
		    outfp << vcl_setprecision(DOUBLE_PRECISION) << cell_bnd->loc() << " ";
	    }
	    else
	    {
		    outfp << "-1 -1 ";
	    }

	    outfp << vcl_setprecision(DOUBLE_PRECISION) << (*iel)->bnd_intersect_pos() << " ";

	    outfp  << (*iel)->hasPropagated() << " " << (*iel)->isValid() << " " << (*iel)->isHidden() << " ";
	    outfp << vcl_setprecision(DOUBLE_PRECISION) << (*iel)->origin().x() << " " << (*iel)->origin().y() << " ";

	    // to save nu for point-line ishock link
        if ((*iel)->type() == 1)
        {
	        dbsk2d_ishock_pointpoint* pp = (dbsk2d_ishock_pointpoint*)(*iel);
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << pp->n() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << pp->u() << " ";
        }
        else if ((*iel)->type() == 2)
        {
	        dbsk2d_ishock_pointline* pl = (dbsk2d_ishock_pointline*)(*iel);
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << pl->nu() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << pl->u() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << pl->n() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << pl->ldelta() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << pl->rdelta() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << pl->l() << " ";
        }
        else if ((*iel)->type() == 4)
        {
	        dbsk2d_ishock_lineline* ll = (dbsk2d_ishock_lineline*)(*iel);
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << ll->ur() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << ll->phi() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << ll->ul() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << ll->sigma() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << ll->thetaL() << " " << ll->thetaR() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << ll->lL() << " " << ll->lR() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << ll->N1L() << " " << ll->N1R() << " " << ll->N2L() << " " << ll->N2R() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << ll->Al().x() << " " << ll->Al().y() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << ll->Bl().x() << " " << ll->Bl().y() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << ll->Ar().x() << " " << ll->Ar().y() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << ll->Br().x() << " " << ll->Br().y() << " ";
        }
        else if ((*iel)->type() == 8)
        {
	        dbsk2d_ishock_contact* c = (dbsk2d_ishock_contact*)(*iel);
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << c->n() << " ";
	        outfp << vcl_setprecision(DOUBLE_PRECISION) << c->origin().x() << " " << c->origin().y() << " ";
        }
        outfp << vcl_endl;
    }

    outfp << "# ==============OTHER INFO==============" << vcl_endl;
    outfp << "# Other info (pshocks in ishock node):" << vcl_endl;
    outfp << "# [ISHOCK_NODE_ID] [NUM_PSHOCK] [PSHOCK_ID_1] ..." << vcl_endl;
    outfp << "[OTHER_INFO_BEGIN]" << vcl_endl;

    for (ishock_node_list_iter inl = nodes.begin(); inl != nodes.end() ; inl++)
    {
        if ((*inl)->pShocks().size() > 0)
        {
            outfp << (*inl)->id() << " ";
            outfp << (*inl)->pShocks().size() << " ";
            ishock_edge_list pshocks = (*inl)->pShocks();
            for (ishock_edge_list_iter iel = pshocks.begin(); iel != pshocks.end() ; iel++)
            {
                outfp << (*iel)->id() << " ";
            }
            outfp << vcl_endl;
        }
    }


    outfp << "# ==============SHOCK MAP==============" << vcl_endl;
    outfp << "# List of shocks formed by this element mapped by the boundary parameter:" << vcl_endl;
	outfp << "#[BELM_ID] [BND_KEY_ETA] [BND_KEY_TYPE] [BND_KEY_FTYPE] [EDGE_ID]" << vcl_endl;
    outfp << "[SHOCK_MAP_BEGIN]" << vcl_endl;

    for (vcl_vector< dbsk2d_ishock_belm* >::iterator it = belms.begin(); it != belms.end(); it++)
    {
        outfp << (*it)->id() << " ";
	    for (bnd_ishock_map_iter map_it = (*it)->shock_map().begin() ; map_it != (*it)->shock_map().end() ; map_it++)
	    {
		    dbsk2d_ishock_bnd_key key = (*map_it).first;
		    outfp << vcl_setprecision(DOUBLE_PRECISION) << key.s_eta << " " ;
		    outfp << key.type << " ";
		    outfp << key.ftype << " ";
		    outfp << (*map_it).second->id() << " ";
	    }
	    outfp << vcl_endl;
    }

	outfp << "# ==============SHOCK GRAPH==============" << vcl_endl;
	outfp << "# Shock graph parameters:" << vcl_endl;
	outfp << "#[NEXT_AVAILABLE_ID]" << vcl_endl;
	outfp << "[SHOCK_GRAPH_BEGIN]" << vcl_endl;

	outfp << _ishock_graph->getAvailableID() << " ";
	outfp << _boundary->getAvailableID() << " ";
	outfp << vcl_endl;

    outfp.close();
    return true;
}

