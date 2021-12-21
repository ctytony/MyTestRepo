//All the codes in the software are subject to the GNU General Public License("GPL") or 
//GNU Lesser General Public License("LGPL").
//Please see the GNU and LPGL Web sites to view the terms of each license.


#ifndef __MTNSSPRF__
#define __MTNSSPRF__

//#include "mtn_def.h"
#include "mtndefin.h"

#define __MAX_SS_PROF_BLK__     20
#define MAX_ARBITRARY_PROF      8000


extern double __ss_prof_get_dist_at_blk(unsigned short usBlkNo);

#ifdef A_MPI_
extern short gpf_mpi_trigger_motion_scope_channel(MPI_MCB *stpMotor, MPI_SCOPE_BLK *stpScopeBlk);
extern MTN_SHORT mtn_mpi_ss_prof_setup_traj(MPI_MCB *stpMotor, unsigned short usCurrBlkSample);
extern MTN_SHORT mtn_mpi_ss_prof_trigger_move_once(MPI_MCB *stpMotor, 
                                        unsigned short usCurrBlkSample, 
                                        unsigned short uPosnTol,
                                        unsigned short uProcessBlk);
extern MTN_SHORT mtn_mpi_ss_prof_trigger_move_twice(MPI_MCB *stpMotor, 
                                        unsigned short usCurrBlkSample, 
                                        unsigned short uPosnTol,
                                        unsigned short uProcessBlk);
#endif // A_MPI_
                                        
extern MTN_SHORT __init_continuous_ss_prof_cmd__(short usTotalBlk, short sDebugFlag, unsigned short *pusTotalMotionSample);
extern MTN_SHORT __init_ss_prof_blk_from_file__(char *strProfSsFile, unsigned short *pusTotalBlk);

extern short __mtn_read_unit_prof_jerk(char strFilenameProfile[], float *pfUnitProf, unsigned short *usTotalExcitingSample);
extern short __mtn_write_prof_jerk(char strFilenameProfile[], float *pfUnitProf, unsigned short usTotalExcitingSample);
extern short __mtn_read_unit_prof_vel(char strFilenameProfile[], float *pfUnitProf, unsigned short *usTotalExcitingSample);
extern short __mtn_write_prof_vel(char strFilenameProfile[], float *pfUnitProf, unsigned short usTotalExcitingSample);
extern short __mtn_read_unit_prof_posn(char strFilenameProfile[], float *pfUnitProf, unsigned short *usTotalExcitingSample);
extern short __mtn_write_prof_posn(char strFilenameProfile[], float *pfUnitProf, unsigned short usTotalExcitingSample);
extern short __mtn_read_unit_prof_acc(char strFilenameProfile[], float *pfUnitProf, unsigned short *usTotalExcitingSample);

#endif
