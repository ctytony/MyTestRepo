//MSGDEF.h v3.1.T575

#ifndef MSG_DEF_H
#define MSG_DEF_H

#define WM_CLICK_KEYBOARDSHORTCUT       (WM_USER+100)

enum EN_MSG_ID	//v3.1.T383 rename
{
	MSG_LANGUAGE = 0,						//0:English
	MSG_SETUP,								//1:Setup
	MSG_PROGRAM,							//2:Program
	MSG_RUN,								//3
	MSG_CONFIGURE,							//4:Configure
	MSG_VISION_CALI,						//5:Vision Calibration
	MSG_SHUTDOWN,							//6:Shut Down
	MSG_TEACH_RECIPE,						//7:Teach Recipe
	MSG_SINGLE_BOND,						//8:Single Bond
	MSG_AUTO_BOND,							//9:Auto Bond
	MSG_SYSTEM_CONFIG,						//10:System Configure
    MSG_UTILITY,							//11:Utility
    MSG_DEBUG,								//12:Debug
    MSG_DUMMY_PROGRAM,						//13:Dummy Program
    MSG_START_DUMMY_PROGRAM,				//14:Press start button to teach dummy program.
    MSG_MOVE_TO_TOP_LEFT_POS,				//15:Move table to left/top position.
    MSG_MOVE_TO_BOTTOM_RIGHT_POS,			//16:Move table to right/bottom position.
    MSG_NEXT_TO_CONTINUE,					//17:Press next button to continue.
    MSG_DUMMY_PGM_COMPLETED,				//18:Complete teaching dummy program.
    MSG_RECIPE_MANAGE,						//19:Program Manage
    MSG_LOAD_RECIPE,						//20:Load Recipe
    MSG_SAVE_RECIPE,						//21:Save Recipe
    MSG_CLEAR_RECIPE,						//22:Clear Recipe
    MSG_HELP_MSG,							//23:Help Message
    MSG_ERROR_MSG,							//24:Error Message
    MSG_WARNING_MSG,						//25:Warning Message
    MSG_BOND_PARAMETER,						//26:Bond Parameter	//v3.1.T359 rename
    MSG_LOOP_PARAMETER,						//27:Loop Parameter	//v3.1.T359 rename
    MSG_BALL,								//28
    MSG_WEDGE,								//29
    MSG_CTACT_TIME,							//30
    MSG_CTACT_POWER,						//31
	MSG_CTACT_FORCE,						//32
    MSG_BASE_TIME,							//33
    MSG_BASE_POWER,							//34
    MSG_BASE_FORCE,							//35
    MSG_RELEASE_TIME,						//36
    MSG_RELEASE_POWER,						//37
    MSG_RELEASE_FORCE,						//38
    MSG_POWER_EQUALIZATION,					//39
    MSG_FORCE_EQUALIZATION,					//40
    MSG_DONE,								//41:Done
    MSG_WIRE_OFFSET,						//42:Wire Offset	//v3.0.T429 rename
    MSG_LOOP_GROUP,							//43:Loop Group
    MSG_LOOP_TYPE,							//44
    MSG_LOOP_HEIGHT,						//45:Loop Height (um)
    MSG_REV_DIST,							//46:Reverse Distance (%)
    MSG_REV_ANGLE,							//47:Reverse Angle (Deg)
    MSG_LOOP_HT_CORRECT,					//48:Loop Height Correction (um)
    MSG_KINK2_SPAN_LEN,						//49:Span Length	//v3.1.T145 rename
    MSG_KINK2_HT_FACTOR,					//50:2nd Kink Height Factor (%)
    MSG_PULL_DIST,							//51:Pull Distance (um)
    MSG_REV_ARC_FACTOR,						//52	//v3.1.T407 add
    MSG_LOOP_BASE_MODE,						//53:Loop Base Mode
    MSG_SPAN_LENGTH_UNIT,					//54:Span Length Unit
    MSG_STANDBY_POWER,						//55
    MSG_REV_ARC_DELAY,						//56	//v3.1.T407 add
    MSG_BOND_SETUP,							//57
    MSG_REFERENCE,							//58:Reference
    MSG_TYPE,								//59:Type
    MSG_CTACT_LEVEL,						//60:Contact Level
    MSG_LEAD,								//61:Lead
    MSG_DIE,								//62:Die
    MSG_GND,								//63:Gnd
    MSG_MOVE_TO_CTACT_POS,					//64:Move table to contact position
    MSG_CTACT_SRCH_SUCCESS,					//65:Contact search successful.
    MSG_CTACT_SRCH_FAILURE,					//66:Contact search failure.
    MSG_NEW_CTACT_LEVEL,                    //67:New contact level is: 
    MSG_BTOS_CURRENT,                       //68:Current (XY):
    MSG_BTOS_NEW,                           //69:New (XY):
    MSG_MOVE_CURSOR_TO_CAP_MARK,			//70:Move cursor to capillary mark
    MSG_BOND_TIP_OFFSET_UPDATED,            //71:Bond tip offset updated!
    MSG_LIGHT_PARA,							//72:Lighting Parameters
    MSG_BINARY,								//73:Binary
    MSG_GREY,								//74:Grey
    MSG_CANCEL,								//75:Cancel
    MSG_START_TO_TEACH_ALIGN_PT,			//76:Press "Start" to teach alignment point.
    MSG_MOVE_TO_LEAD_1ST_ALIGN_PT,			//77:Move table to lead 1st alignment point.
    MSG_MOVE_TO_LEAD_2ND_ALIGN_PT,			//78:Move table to lead 2nd alignment point.
    MSG_MOVE_TO_DIE_1ST_ALIGN_PT,			//79:Move table to die 1st alignment point.
    MSG_MOVE_TO_DIE_2ND_ALIGN_PT,			//80:Move table to die 2nd alignment point.
    MSG_ADJUST_LIGHT_FOR_CLEAR_IMAGE,       //81:Adjust lighting for clear image.
    MSG_TEACH_ALIGN_POINT_COLON,            //82:Teaching alignment point:
    MSG_TEACH_DIE_ALIGN_POINT,              //83:Teaching die alignment point
    MSG_CONTINUE_WHEN_READY,				//84:If ready press continue button
    MSG_NEXT_TO_TEACH_PATTERN,				//85:Press 'Next' to teach vision pattern
    MSG_TEACH_ALIGN_PT_FINISH,				//86:Teach alignment point finish.
    MSG_START_TO_TEACH_VISION_PATTERN,		//87:Press "Start" to teach vision Pattern
    MSG_TEACH_VISION_PATTERN,               //88:Teaching vision pattern:
    MSG_TEACH_DIE_VISION_PATTERN,           //89:Teaching die vision pattern
    MSG_MOVE_TO_LEAD_1ST_PAT_PT,			//90:Move table to lead 1st pattern point.
    MSG_MOVE_TO_LEAD_2ND_PAT_PT,			//91:Move table to lead 2nd pattern point.
    MSG_MOVE_TO_DIE_1ST_PAT_PT,				//92:Move table to die 1st pattern point.
    MSG_MOVE_TO_DIE_2ND_PAT_PT,				//93:Move table to die 2nd pattern point.
    MSG_TEACH_VISION_PATTERN_FINISH,        //94:Teach vision pattern finish.
    MSG_PRESS_NEXT_TO_TEACH_WIRE_LAYOUT,    //95:Press 'Next' to teach wire layout
    MSG_MOVE_TO_1ST_BOND_PT,				//96:Move table to wire 1st bond point.
    MSG_MOVE_TO_2ND_BOND_PT,				//97:Move table to wire 1st bond point.
    MSG_REV_HTFACTOR2,						//98	//v3.1.T407 add
    MSG_PRE_KINK_HTFACTOR2,					//99	//v3.1.T407 add
    MSG_PRE_KINK_ARC_FACTOR,				//100	//v3.1.T407 add
    MSG_PRE_KINK_ARC_DELAY,					//101	//v3.1.T407 add
    MSG_KINK2_HTFACTOR2,					//102	//v3.1.T407 add
    MSG_KINK2_ARC_FACTOR,					//103	//v3.1.T407 add
    MSG_KINK2_ARC_DELAY,					//104	//v3.1.T407 add
    MSG_KINK3_HTFACTOR2,					//105	//v3.1.T407 add
    MSG_START_TO_TEACH_SEQ,					//106
    MSG_MOVE_TO_TOP_RIGHT_ALIGN_PT,			//107
    MSG_KINK3_ARC_FACTOR,					//108	//v3.1.T407 add
	MSG_KINK3_ARC_DELAY,					//109	//v3.1.T407 add
    MSG_MOVE_TO_TOP_LEFT_ALIGN_PT,			//110
    MSG_MOVE_TO_BOTTOM_LEFT_ALIGN_PT,		//111
    MSG_TEACH_SEQ_FINISH,					//112
    MSG_KINK2_HT,							//113	//v3.1.T407 add
    MSG_SELECT_BOND_SEQ,					//114
    MSG_CALI_TO_LEARN_VISION_FACTOR,		//115:Press 'Calibrate' to learn vision factor.
    MSG_MOVE_TO_UNIQUE_PATTERN_PT,			//116:Move table to a unique pattern point.
    MSG_ROTATE_ANGLE_TOO_LARGE,				//117:Camara rotate angle too large!
    MSG_VISION_SRCH_OFFSET_ZERO,			//118:Vision search offset is zero!
    MSG_VISION_FACTOR_CALI_FAIL,			//119:Vision factor calibration fail!
	MSG_EDIT_RECIPE,						//120:Edit Recipe
    MSG_START_TO_EDIT_ALIGN_PT,				//121:Press "Start" to edit alignment point.
    MSG_EDIT_ALIGN_PT,						//122:Edit Alignment Point
    MSG_EDIT_ALIGN_PT_COLON,				//123:Editing alignment point:
    MSG_KINK3_HT,							//124	//v3.1.T407 add
    MSG_EDIT_ALIGN_PT_FINISH,				//125:Edit alignment point finish.
    MSG_KINK2_SPANLEN2,						//126	//v3.1.T407 add
    MSG_NEXT_TO_EDIT_VISION_PATTERN,		//127:Press 'Next' to edit vision pattern
    MSG_EDIT_VISION_PATTERN,				//128:Editing vision pattern:
    MSG_EDIT_DIE_VISION_PATTERN,			//129:Editing die vision pattern
    MSG_EDIT_VISION_PATTERN_FINISH,			//130:Edit vision pattern finish.
    MSG_NEXT_TO_EDIT_WIRE,					//131:Press 'Next' to edit wire layout
    MSG_START_TO_TEACH_FIRE_LEVEL,			//132:Press 'Start' to teach fire level.
    MSG_UP_DOWN_TO_ADJUST_FIRE_LEVEL,		//133:Press 'UP/Down' to adjust fire level	//v3.0.T454 edit
    MSG_KEY_IN_FIRE_LEVEL,					//134:Or directly key in fire level.
    MSG_CONFIRM_TO_UPDATE_FIRE_LEVEL,		//135:Press "Confirm" update new fire level.
    MSG_DO_MANUAL_ALIGN,					//136:Please do manual alignment.
    MSG_MOVE_TO_1ST_ALIGN_PT,				//137:Move table to 1st alignment point.
    MSG_MOVE_TO_2ND_ALIGN_PT,				//138:Move table to 2nd alignment point.
    MSG_CONFIG_SERVO,						//139:Servo System Configure
    MSG_MHS_PARA,							//140:Material Handler System Parameter
    MSG_SERVO_CALI,							//141:Servo System Calibration
    MSG_KINK3_SPANLEN2,						//142	//v3.1.T407 add
    MSG_REV_SPANLEN2,						//143
    MSG_PRE_KINK_SPANLEN2,					//144
    MSG_FORCE_VERIFICATION_SUCEESS,			//145:Force verification success.
    MSG_FORCE_VERIFICATION_FAIL,			//146:Force verification fail.
    MSG_MOVE_TO_DUMMY_BOND_POS,				//147:Move table to dummy bond position.
    MSG_MAKE_SURE_LIGHT_OK,					//148:Make sure lighting is OK.
    MSG_RESERVE_149,						//149
    MSG_RESERVE_150,						//150
    MSG_RESERVE_151,						//151
    MSG_RESERVE_152,						//152
    MSG_FORCE_GM,							//153:Force (gm)
    MSG_WIRE_CLAMP,							//154:Wire Clamp
    MSG_CTACT_SRCH,							//155:Contact Search
    MSG_BOND_TIP_OFFSET,					//156:Bond Tip Offset
    MSG_FIRE_LEVEL,							//157:Fire Level
    MSG_RESERVE_158,						//158
    MSG_DUMMY_BOND,							//159:Dummy Bond
    MSG_GENERAL,							//160:General
    MSG_STATISTICS,							//161:Statistics
    MSG_ACCELERATION,						//162:Acceleration
    MSG_WINDOW_CLAMP,						//163:Window Clamper
    MSG_INPUT,								//164
    MSG_TRACK,								//165:Track
    MSG_OUTPUT,								//166
    MSG_NEXT,								//167:Next
    MSG_PREV,								//168:Previous
    MSG_STOP_BOND,							//169:Stop Bond
    MSG_BTO,								//170:BTO
    MSG_CORRECT_BOND,						//171:Correct Bond
    MSG_BALL_DETECT,						//172:Ball Detect
    MSG_AUTO_INDEX,							//173:Auto Index
    MSG_DRY_RUN,							//174
    MSG_NO_VISION,							//175:No Vision
    MSG_COMPLETE,							//176:Complete
    MSG_BOND_POINT,							//177:Bond Point
    MSG_CURRENT_WIRE,						//178:Current Wire #
    MSG_RESERVE_179,						//179
    MSG_RESERVE_180,						//180
    MSG_RESERVE_181,						//181
    MSG_RESERVE_182,						//182
    MSG_RESERVE_183,						//183
    MSG_RESERVE_184,						//184
    MSG_FORWARD,							//185:Forward
    MSG_BACKWARD,							//186:Backward
    MSG_ON,									//187:On
    MSG_OFF,								//188:Off
    MSG_RESERVE_189,						//189
    MSG_RESERVE_190,						//190
    MSG_CLAMPER,							//191:Clamper
    MSG_RESERVE_192,						//192
    MSG_PARAMETERS,							//193:Parameters
    MSG_RESERVE_194,						//194
    MSG_RESERVE_195,						//195
    MSG_CLAMP_POS_COLON,					//196:Clamp Position:
    MSG_OPEN_POS_COLON,						//197:Open Position:
    MSG_HOME,								//198:Home
    MSG_CLOSE,								//199:Close
    MSG_OPEN,								//200:Open
    MSG_CONFIG_VISION,						//201:Vision System Configure
    MSG_MOTION_DELAY,						//202:Motion Delay
    MSG_SHORT_DIST,							//203:Short Distance (ms):
    MSG_LONG_DIST,							//204:Long Distance (ms):
    MSG_SRCH_MODE,							//205:Search Mode
    MSG_SINGLE_PR,							//206:Single PR:
    MSG_PRE_MOVE,							//207:Pre Move:
    MSG_DISABLE_2,							//208:Disable
    MSG_ENABLE_2,							//209:Enable
    MSG_ACTION,								//210:Action
    MSG_MODULE_COLON,						//211:Module:
    MSG_PRE_TRACK,							//212:PreTrack
    MSG_POST_TRACK,							//213:PostTrack
    MSG_ONLOAD_TRACK,						//214:Onload Track
    MSG_DEBUG_COLON,						//215:Debug:
    MSG_RESERVE_216,						//216
    MSG_MAG_INDEX,							//217:MAG Index
    MSG_ELEVATOR,							//218:Elevator
    MSG_ARM_STANDBY,						//219:Arm Standby Position:
    MSG_ARM_SENSOR_DIST,					//220:Arm Sensor Distance:
    MSG_MAG_THICKNESS,						//221:Magazine Thickness:
    MSG_MAG_BUFFER_SIZE,					//222:Magazine Buffer Size:
    MSG_FIRST_EMPTY_MAG,					//223:First Empty Mag
    MSG_FIRST_FULL_MAG,						//224:First Full Mag
    MSG_EMPTY_MAG_POS,						//225:Empty Mag Position
    MSG_FULL_MAG_POS,						//226:Full Mag Position
    MSG_MAG_BUFFER,							//227:Magazine Buffer
    MSG_MAG_STOREY,							//228:Magazine Storey:
    MSG_EMPTY_MAG,							//229:Empty Mag
    MSG_FULL_MAG,							//230:Full Mag
    MSG_LEARN_POS,							//231:Learn Pos
    MSG_1ST_SLOT_POS,						//232:1st Slot Pos
    MSG_SLOT_NUM,							//233:Total Slot:
    MSG_SLOT_PITCH,							//234:Slot Pitch:
    MSG_TRACK_SPACE,						//235:Track Space:
    MSG_POS_ADJUST,							//236:Position Adjust
    MSG_ADJUST_ITEM,						//237:Adjust Item:
    MSG_MAG_POS,							//238:Magazine Position:
    MSG_ADJUST_STEP,						//239:Adjust Step
    MSG_MOVE,								//240:Move
    MSG_MORE,								//241:More
    MSG_LESS,								//242:Less
    MSG_CUR_SLOT,							//243:Current Slot:
    MSG_SLOT_VERIFY,						//244:Slot Verify
    MSG_SPEED,								//245:Speed
    MSG_CLAMP_STEPPER,						//246:Clamp Stepper
    MSG_CLAMPER_COLON,						//247:Clamper:
    MSG_INDEX_STEPPER,						//248:Index Stepper
    MSG_ELEVATOR_STEPPER,					//249:Elevator Stepper
    MSG_INDEX_SPEED_COLON,					//250:Index Speed:
    MSG_ELEVATOR_SPEED_COLON,				//251:Elevator Speed:
    MSG_START,								//252:Start
    MSG_STOP,								//253:Stop
    MSG_LOW,								//254:Low
    MSG_MIDDLE,								//255:Middle
    MSG_HIGH,								//256:High
    MSG_CLEAR,								//257:Clear
    MSG_UP,									//258:Up
    MSG_DOWN,								//259:Down
    MSG_ARM_CYLINDER_COLON,					//260:Arm Cylinder:
    MSG_ROTARY_CYLINDER_COLON,				//261:Rotary Cylinder:
    MSG_BOTH,								//262:Both
    MSG_INITIALIZE,							//263:Initialize
    MSG_RECOVER,							//264:Recover
    MSG_CALIBRATE,							//265:Calibrate
    MSG_VERIFY,								//266:Verify
    MSG_FINISH,								//267:Finish
    MSG_FACTORS_COLON,						//268:Factors:
    MSG_APPLY_FORCE_COLON,					//269:Apply Force:
    MSG_MEASURE_FORCE_COLON,				//270:Measure Force:
    MSG_MEASURE_WCL_FORCE,					//271:Measure wire clamp force with force gauge.
    MSG_KEYIN_MEASURED_FORCE,				//272:Keyin measured force.
    MSG_STEP_1,								//273:Step 1:
    MSG_STEP_2,								//274:Step 2:
    MSG_CALI_FINISH,						//275:Calibration finish.
    MSG_VERIFY_FINISH,						//276:Verification finish.
    MSG_CURRENT,							//277:Current:
    MSG_NEW_COLON,							//278:New:
    MSG_OPERATION,							//279:Operation
    MSG_RESERVE_280,						//280:Fast
    MSG_VERY_HIGH,							//281:Very High
    MSG_MOTION_TYPE_COLON,					//282:Motion Type:
    MSG_SPEED_COLON,						//283:Speed:
    MSG_EMPTY_MOTION,						//284:Empty Motion
    MSG_LOAD_MOTION,						//285:Load Motion
    MSG_INDEX_MOTION,						//286:Index Motion
    MSG_SRCH_MOTION,						//287:Search Motion
    MSG_MEASURE_BH_FORCE,					//288:Measure bond head force with force gauge.
    MSG_ADVANCE_BOND,						//289:Advance Bond
    MSG_SRCH_HT,							//290:Search Height
    MSG_SRCH_THRES,							//291:Search Threshold
    MSG_SRCH_SPEED,							//292:Search Speed
    MSG_SPARK,								//293:Spark
    MSG_TAIL_LENGTH_COLON,					//294:Tail Length:
    MSG_SPARK_DELAY_COLON,					//295:Spark Delay:
    MSG_TBL_POS_OUT_OF_LIMIT,				//296:Table position out of limit!
    MSG_TBL_JOG_SPEED,						//297:Table Jogging Speed:
    MSG_INCREASE,							//298:Increase
    MSG_DECREASE,							//299:Decrease
    MSG_LF_INDEX,							//300:LF Index
    MSG_UNIT_INFO,							//301:Unit Information
    MSG_CUR_UNIT_COLON,						//302:Current Unit:
    MSG_ROW,								//303
    MSG_COLUMN,								//304
    MSG_UNIT_LAYOUT,						//305:Unit Layout
    MSG_BOND_SHIFT,							//306:Bond Shift (um)
    MSG_APPLY,								//307:Apply
    MSG_TAIL_CLOSE,							//308:WCL Tail Close Pretrig (ms)
    MSG_BOND_TIMEOUT,						//309:Bond Timeout!
    MSG_SERVO_ERROR,						//310:Servo Error!
    MSG_SPARK_FAILURE,						//311:Spark Failure!
    MSG_1ST_BOND_NONSTICK,					//312:1st Bond Nonstick!
    MSG_2ND_BOND_NONSTICK,					//313:2nd Bond Nonstick!
    MSG_POWER_DELAY,						//314:Power Delay (ms)
    MSG_TAIL_CREEP,							//315:Tail Shift (um)
    MSG_PUSH_DELAY_COLON,					//316:Push Delay:
    MSG_KICKER,								//317:Kicker:
    MSG_HOLDER,								//318:Holder:
    MSG_PUSHER,								//319:Pusher
    MSG_CYLINDER,							//320:Cylinder
    MSG_BOND_CALC_ERR,						//321:Bond Calculate Error!
    MSG_PUSHER_STEPPER,						//322:Pusher Stepper
    MSG_START_POS_COLON,					//323:Start Position:
    MSG_PUSH,								//324:Push
    MSG_BACK,								//325:Back
    MSG_ONLOAD_STEPPER,						//326:Onload Stepper
    MSG_LF_THICKNESS,						//327:LF Thickness:
    MSG_PUSH_DIST,							//328:Push Distance:
    MSG_FACTOR_OUT_RANGE,					//329:Calibration factor out of range!
    MSG_CALI_AGAIN,							//330:Please calibration again!
    MSG_NECK_HT,							//331:Neck Height (um)
    MSG_OPERATOR,							//332:Operator
    MSG_PROCESS,							//333:Process
    MSG_SERVICE,							//334:Service
    MSG_LOOP_PEAK_CLOSE,					//335:WCL Loop Peak Close Pretrig (ms)
    MSG_HEATER_TEMP_OUT_RANGE,				//336:Heater temperature out of range!
    MSG_CLAW,								//337:Claw
    MSG_LIMIT,								//338:Push Limit
    MSG_TRAJ_FACTOR,						//339:Trajectory Factor (%)
    MSG_BSD_NETWORK_ERR,					//340:BSD Network Error.
    MSG_BSD_SOCKET_ERR,						//341:BSD Socket Error.
    MSG_BSD_COMM_ERR,						//342:BSD Communication Error.
    MSG_BSD_VERSION_ERR,					//343:BSD Version Error.
    MSG_EFO_INIT_ERR,						//344:EFO Init Error.
    MSG_CONFIG_BSD,							//345:Stick Detection Configure
    MSG_HARDWARE_PARA,						//346:Hardware Parameters
    MSG_APPLY_VOLTAGE_COLON,				//347:Apply Voltage (V):
    MSG_DIE_DETECT_THRES_COLON,				//348:Die Detect Threshold (V):
    MSG_LEAD_DETECT_THRES_COLON,			//349:Lead Detect Threshold (V):
    MSG_TAIL_SHORT_THRES_COLON,				//350:Tail Short Threshold:
    MSG_NONSTICK_THRES_COLON,				//351:Nonstick Threshold:
    MSG_BOND_DETECT_PARA,					//352:Bond Detect Parameters
    MSG_TAIL_TOO_SHORT,						//353:Tail Too Short!
    MSG_PRE_CLAMP_POS_COLON,				//354:Pre Clamp Position:
    MSG_INDEX_TIMEOUT,						//355:Lead Frame Index Failure!
    MSG_UPDATE,								//356:Update
    MSG_THREAD_WIRE_POS,					//357:Thread Wire Position
    MSG_CLEAR_TRACK,						//358:Clear Track
    MSG_SKIP,								//359:Skip
    MSG_ONLOAD_PUSH_KICKER_WRONG,			//360:Onload Push Kicker Wrong!
    MSG_ONLOAD_PICKER_JAM,					//361:Onload Picker Jam!
    MSG_ONLOAD_PULLY_JAM,					//362:Onload Selecter Wrong!
    MSG_CLAW_MISSING_STEP,					//363:Claw Missing Steps, Please Home it!
    MSG_OFFLOAD_KICKER_WRONG,				//364:Offload Kicker Wrong!
    MSG_OFFLOAD_KICKER_JAM,					//365:Offload Kicker Jam!
    MSG_OFFLOAD_CLAW_WRONG,					//366:Offload Claw Wrong!
    MSG_OFFLOAD_CLAW_JAM,					//367:Offload Claw Jam!
    MSG_OFFLOAD_INNER_PUSHER_WRONG,			//368:Offload Inner Pusher Wrong!
    MSG_OFFLOAD_PUSHER_JAM,					//369:Offload Inner Pusher Jam!
    MSG_OFFLOAD_OUTER_PUSHER_WRONG,			//370:Offload Outer Pusher Wrong!
    MSG_OFFLOAD_OUTER_PUSHER_JAM,			//371:Offload Outer Pusher Jam!
    MSG_OFFLOAD_INNER_HOLDER_WRONG,			//372:Offload Inner Holder Wrong!
    MSG_OFFLOAD_INNER_HOLDER_JAM,			//373:Offload Inner Holder Jam!
    MSG_OFFLOAD_OUTER_HOLDER_WRONG,			//374:Offload Outer Holder Wrong!
    MSG_OFFLOAD_OUTER_HOLDER_JAM,			//375:Offload Outer Holder Jam!
    MSG_CONTROL,							//376:Control
    MSG_PRE_IMPACT_FORCE,					//377:Pre Impact Force
    MSG_ADVANCE_LOOP,						//378:Advance Loop
    MSG_SRCH_DELAY,							//379:Search Delay
    MSG_PRE_HEAT_TIME,						//380:Pre Heat Time:
    MSG_PR_PASS_PERCENT,					//381:PR Pass Score (%):
    MSG_RESET,								//382:Reset
    MSG_MACHINE_ID,							//383:Machine ID		//V1.1.W258 edit
    MSG_START_TIME,							//384:Start Time:
    MSG_OUTPUT_COLON,						//385:Output:
    MSG_PRODUCTION,							//386:Production:
    MSG_STANDBY,							//387:Standby:
    MSG_ASSIST,								//388:Assist:
    MSG_FAILURE,							//389:Failure:
    MSG_WIRE,								//390:Wire:
    MSG_TOOL_USAGE,							//391:Tool Usage:
    MSG_EFO_OPEN,							//392:EFO Open:
    MSG_SHORT_TAIL,							//393:Short Tail:
    MSG_ASSISTANCE,							//394:Assistance
    MSG_THRESHOLD,							//395:Threshold
    MSG_REF_INFO,							//396:Reference Information
    MSG_CTACT_PARA,							//397:Contact Parameters
    MSG_VISION_FACTOR,						//398:Vision Factors
    MSG_SETTING,							//399:Setting
    MSG_MAGNIFICATION,						//400:Magnification
    MSG_PATTERN_SIZE_X,						//401:Pattern Size X
    MSG_PATTERN_SIZE_Y,						//402:Pattern Size Y
    MSG_TIME_MS,							//403:Time (ms)
    MSG_POWER_DAC,							//404:Power (Dac)
    MSG_CONTINUE,							//405:Continue
    MSG_TEACH_ALIGN_PT,						//406:Teach Alignment Point
    MSG_REF_NUM,							//407:Reference Number
    MSG_LENS_SETTING,						//408:Lens Setting
    MSG_DIE_NUM,							//409:Die Number
    MSG_PATTERN_PARA,						//410:Pattern Parameters
    MSG_VISION_ALGORITHM,					//411:Vision Algorithm
    MSG_LOCATE_MODE,						//412:Locate Mode
    MSG_GREY_LEVEL,							//413:Grey Level
    MSG_RESERVE_414,						//414
    MSG_DUAL_PT,							//415:Dual Point
    MSG_SINGLE_PT,							//416:Single Point
    MSG_STANDARD,							//417:Standard
    MSG_FLAT,								//418:Flat
    MSG_MODE_A,								//419:Mode A
    MSG_MODE_B,								//420:Mode B
    MSG_BOND_MODE,							//421:Bond Mode
    MSG_BOND_CTRL,							//422:Bond Control
    MSG_MOTOR,								//423:Motor
    MSG_SYSTEM,								//424:System
    MSG_NEW,								//425:New
    MSG_EDIT,								//426:Edit
    MSG_DELETE,								//427:Delete
    MSG_INSERT,								//428:Insert
    MSG_TEACH_WIRE_LAYOUT,					//429:Teach Wire Layout
    MSG_WIRE_PROPERTY,						//430:Wire Property
    MSG_1ST_BOND_ON,						//431:1st Bond On
    MSG_2ND_BOND_ON,						//432:2nd Bond On
    MSG_WIRE_LAYOUT,						//433:Wire Layout
    MSG_WIRE_NUM,							//434
    MSG_RESERVE_435,						//435
    MSG_SEQ_TYPE,							//436:Sequence Type
    MSG_NO_OF_ROW_COL,						//437
    MSG_RESERVE_438,						//438
    MSG_AHEAD_COL,							//439:Ahead Column
    MSG_BOND_SEQ,							//440:Bonding Sequence
    MSG_TRACK_TYPE,							//441:Track Type
    MSG_RESERVE_442,						//442:Subunit
    MSG_NONE,								//443:None
    MSG_MATRIX,								//444:Matrix
    MSG_HYBRID,								//445:Hybrid
    MSG_DIE_AHEAD,							//446:Die Ahead
    MSG_DIE_LEAD_AHEAD,						//447:Die Lead Ahead
    MSG_GENERAL_PROFILE,					//448:General Profile
    MSG_THREAD_WIRE_OPEN_FORCE,				//449:Thread Wire Open
    MSG_THREAD_WIRE_CLOSE_FORCE,			//450:Thread Wire Close
    MSG_HOLD_WIRE_CLOSE_FORCE,				//451:Hold Wire Close
    MSG_BOND_PROFILE,						//452:Bond Profile
    MSG_PROFILE_TYPE,						//453:Profile Type
    MSG_1ST_STEP_FORCE,						//454:1st Step Force
    MSG_1ST_STEP_RAMP_TIME,					//455:1st Step Ramp Time
    MSG_1ST_STEP_HOLD_TIME,					//456:1st Step Hold Time
    MSG_2ND_STEP_FORCE,						//457:2nd Step Force
    MSG_2ND_STEP_RAMP_TIME,					//458:2nd Step Ramp Time
    MSG_2ND_STEP_HOLD_TIME,					//459:2nd Step Hold Time
    MSG_CLAMP_FORCE,						//460:Clamp Force
    MSG_CLAMP_RAMP_TIME,					//461:Clamp Ramp Time
    MSG_BALL_SIT_OPEN_CLOSE,				//462:Ball Sit Open/Close
    MSG_SRCH_HT_OPEN,						//463:Search Height Open
    MSG_PEAK_HT_CLOSE,						//464:Peak Height Close
    MSG_TAIL_HT_CLOSE,						//465:Tail Height Close
    MSG_STOP_ALL,							//466:Stop All
    MSG_PARA_OUT_OF_RANGE,					//467:Parameter out of range!
    MSG_SOFTWARE_VERSION,					//468:Version:
    MSG_1ST_BOND_OPEN,						//469:WCL 1st Search Height Pretrig (ms)
    MSG_2ND_BOND_OPEN,						//470:WCL 2nd Search Height Pretrig (ms)
    MSG_RESERVE_471,						//471
    MSG_ONLOAD_FORK_NOT_EXIST,				//472:Onload Fork Not Exist!
    MSG_OFFLOAD_FORK_NOT_EXIST,				//473:Offload Inner Fork Not Exist!
    MSG_OFFLOAD_OUTER_FORK_NOT_EXIST,		//474:Offload Outer Fork Not Exist!
    MSG_OFFLOAD_FORK_FULL,					//475:Offload Fork Full!
    MSG_OFFLOAD_OUTER_FORK_FULL,			//476:Offload Outer Fork Full!
    MSG_LEADFRAME_GAP_EXIST,				//477:Gap exist between Lf to Lf!
    MSG_IMAGE_TRANSFER_TIME,				//478:Image Transfer Time (ms):
    MSG_DISABLE,							//479:Disable
    MSG_ENABLE,								//480:Enable
    MSG_TRACK_FLATNESS,						//481:Track Flatness
    MSG_START_TO_CHECK_TRACK_FLATNESS,		//482:Press 'Start' to check track flatness!
    MSG_MOVE_TO_INNER_TRACK_LEFT_POS,		//483:Move table to inner track left position.
    MSG_MOVE_TO_INNER_TRACK_MIDDLE_POS,		//484:Move table to inner track middle position.
    MSG_MOVE_TO_INNER_TRACK_RIGHT_POS,		//485:Move table to inner track right position.
    MSG_MOVE_TO_OUTER_TRACK_LEFT_POS,		//486:Move table to outer track left position.
    MSG_MOVE_TO_OUTER_TRACK_MIDDLE_POS,		//487:Move table to outer track middle position.
    MSG_MOVE_TO_OUTER_TRACK_RIGHT_POS,		//488:Move table to outer track right position.
    MSG_CTACT_LEVEL_LIST,					//489:Track level list:
    MSG_NSOP,								//490:BSD
    MSG_SCRUB,								//491:Scrub
    MSG_SCRUB_GROUP,						//492:Scrub Group
    MSG_SCRUB_POS_OFFSET1,					//493:Scrub Offset1 (um)
    MSG_SCRUB_POS_OFFSET2,					//494:Scrub Offset2 (um)
    MSG_CYCLE,								//495:Cycle
    MSG_AMPLITUDE,							//496:Amplitude (um)
    MSG_DIRECTION,							//497:Direction
    MSG_SPEED_PERCENT,						//498:Speed (%)
    MSG_VERTICAL,							//499:Vertical
	MSG_AUTO_SKIP_DIE_UNIT,					//500
    MSG_PR_SRCH_RANGE,						//501:Search Range
    MSG_CONFIG_BOND_PROCESS,				//502:Bond Process Configure
    MSG_BOND_SAFETY,						//503:Safety
    MSG_CAP_WEAR_OUT_LIMIT,					//504:Capillary Wear Out Limit:
    MSG_CAP_USAGE,							//505:Capillary Usage
    MSG_CAP_LIMIT,							//506:Limit:
    MSG_CAP_COUNT,							//507:Count:
    MSG_CAP_WEAR_OUT,						//508:Capillary Wear Out Of Limit.
    MSG_CHANGE_CAP,							//509:Please Change Capillary.
    MSG_RED,								//510:Red
    MSG_SIDE,								//511:Side
    MSG_BLUE,								//512:Blue
    MSG_CALI_FORCE,							//513:Force
    MSG_CONFIRM,							//514:Confirm
    MSG_TAIL_LENGTH,						//515:Tail Length
    MSG_CUR_FIRE_LEVEL,						//516:Current Fire Level
    MSG_NEW_FIRE_LEVEL,						//517:New Fire Level
    MSG_WIRE_BONDED,						//518:Wire Will Be Double Bonded,
    MSG_CONFIRM_TO_CONTINUE,				//519:Please Confirm If Continue Bond.
    MSG_TRAJ_MODE,							//520:Trajectory Mode
    MSG_ALL,								//521:All
    MSG_ODD,								//522:Odd
    MSG_EVEN,								//523:Even
    MSG_BACKUP_LIGHTING,					//524:Backup Lighting
    MSG_ALIGNMENT,							//525:Alignment
    MSG_BOND_POS,							//526:Bond Position
    MSG_ONLOAD_FORK_EMPTY,					//527:Onload Fork Empty!
    MSG_ONLOAD_LEADFRAME,					//528:Please Onload Lead Frame!
    MSG_1ST_BASE_TIME,						//529:Base Time 1
    MSG_1ST_BASE_POWER,						//530:Base Power 1
    MSG_1ST_BASE_FORCE,						//531:Base Force 1
    MSG_2ND_BASE_TIME,						//532:Base Time 2
    MSG_2ND_BASE_POWER,						//533:Base Power 2
    MSG_2ND_BASE_FORCE,						//534:Base Force 2
    MSG_FORK_CAPACITY,						//535:Fork Capacity:
    MSG_INNER_TRACK_OFFLOAD_FORK_FULL,		//536:Inner Track Offload Fork Full!
    MSG_OFFLOAD_LEADFRAME,					//537:Please Offload Lead Frame!
    MSG_OUTER_TRACK_OFFLOAD_FORK_FULL,		//538:Outer Track Offload Fork Full!
    MSG_SHIFT_SPEED,						//539:Shift Speed (%)
    MSG_COAX,								//540:Coax
    MSG_DIE_TOL_FAILURE,					//541:Die Tol Fail.
    MSG_LEAD_TOL_FAILURE,					//542:Lead Tol Fail.
    MSG_VISION_ALIGN_TOL,					//543:Alignment Tolerance
    MSG_REDO_PR,							//544:Redo PR
    MSG_BSD_MAP_MSG,						//545:0: Disable; 1: Enable
    MSG_BUMP_MAP_MSG,						//546
    MSG_BUMP_BALL,							//547	//v3.1.T358 rename
    MSG_BSOB_PARA,							//548:BSOB Parameter	//v3.0.T429 rename
    MSG_BALL_SHAPE,							//549:Shape
    MSG_BOND_BALL_MODE,						//550:Bond Ball Mode
    MSG_BBOS,								//551:Bbos
    MSG_BSOB,								//552:Bsob
    MSG_2ND_BOND_OFFSET,					//553:Position Offset (um)
	MSG_BASE_HT,							//554
	MSG_MOTION_SPEED,						//555
	MSG_SCRUB_DIST,							//556
	MSG_SPAN_LEN,							//557	//v3.1.T383 rename
	MSG_BALL_THICKNESS,						//558
    MSG_BOND_SEQ_ERR,						//559:Bond Sequence Error!
    MSG_WIRE_AND_BALL,						//560:Wire and Ball
    MSG_BALL_ONLY,							//561:Ball Only
    MSG_WIRE_ONLY,							//562:Wire Only
    MSG_BOND_OBJECT,						//563:Bond Object
    MSG_BUMP_WIRE,							//564
    MSG_BUMP_BALL_PARA,						//565	//v3.1.T358 rename
    MSG_NO_ALIGNMENT,						//566
    MSG_START_TO_TEACH_CTACT_LEVEL,			//567:Press 'Start' to teach contact level.
    MSG_TEACH_CTACT_LEVEL,					//568:Teach contact level:
    MSG_LOOP_TOP_MODE,						//569:Loop Top Mode
    MSG_MANUAL_INDEX_LF,					//570:Please Index Lead Frame Manually!
    MSG_USG_CONFIG,							//571:USG
    MSG_USG_RAMP_TIME,						//572:USG Ramp Time
    MSG_THERMAL_EXPANSION,					//573:Thermal Expansion
    MSG_EXPANSION_HANDLE,					//574:Expansion Handle:
    MSG_HANDLE_TIME,						//575:Handle Time:
    MSG_PRE_HEATER,							//576:Pre Heater
    MSG_SENSOR_SWITCH_TIME,					//577:Sensor Switch Time
    MSG_ONLOAD_INNER_TRACK_NO_LF,			//578:Onload Inner Track No Lead Frame.
    MSG_ONLOAD_OUTER_TRACK_NO_LF,			//579:Onload Outer Track No Lead Frame.
    MSG_TRACK_LF_NOT_EXIST,					//580:Track Lead Frame Jam.
    MSG_RESET_ALL_TRACK,					//581:Clear all the track
    MSG_ONLOAD_TRACK_STANDBY,				//582:Onload Track Standby
    MSG_BOND_TRACK_STANDBY,					//583:Bond Track Standby
    MSG_ONLOAD_TRACK_JAM,					//584:Onload Inner Track Jam.
    MSG_ONLOAD_OUTER_TRACK_JAM,				//585:Onload Outer Track Jam.
    MSG_2ND_IMPACT_FORCE,					//586:Impact Force 2
    MSG_STATIC_POWER,						//587:Feed Power
    MSG_CUR_WIRE,							//588:Current Wire:
    MSG_RESTART_WIRE,						//589:Restart Wire:
    MSG_TOTAL_WIRE,							//590:Total Wire:
    MSG_NSOL_SAMPLE_COLON,					//591:NSOL Sample:
    MSG_HEATER,								//592:Heater
    MSG_BOND_PARA,							//593:Bond Para	//v3.1.T359 rename
    MSG_LOOP_PARA,							//594:Loop Para	//v3.1.T359 rename
    MSG_WIRE_USAGE,							//595:Wire Usage
    MSG_BONDED_WIRE,						//596:Wires:
    MSG_SOFTWARE_MANAGER,					//597:Software Manager
    MSG_UPGRADE_SOFTWARE,					//598:Upgrade
    MSG_CUR_VERSION,						//599:Current Version
    MSG_DATE_TIME,							//600
    MSG_SOFTWARE_UPGRADE,					//601:Software Upgrade
    MSG_RESTART_SETTING,					//602:Restart Setting
    MSG_BONDER,								//603:Bonder
    MSG_MHS,								//604:MHS
    MSG_SERVO,								//605:Servo
    MSG_COLD_START_ITEMS,					//606:Cold Start Items:
    MSG_EDIT_TO_EDIT_BOND_PT,				//607:Press 'Edit' to edit wire bond point.
    MSG_NEW_TO_TEACH_NEW_BOND_PT,			//608:Press 'New' to teach new wire bond point.
    MSG_START_TO_EDIT_PROGRAM,				//609:Press 'Start' to edit program
    MSG_PREV_NEXT_TO_CHANGE_WIRE_NUM,		//610:Press 'Previous','Next' to change wire number.
    MSG_BOND_HEAD,							//611:Bond Head
	MSG_RELAX_POS,							//612:Relax Position
	MSG_LOAD_CONTROL_PARA,					//613:Load Control Parameter
	MSG_CALI_POSITION,						//614:Position
	MSG_LOAD,								//615:Load
    MSG_ENGINEER,							//616:Engineer
    MSG_PR_PATTERN,							//617:PR Pattern
    MSG_EDIT_WIRE,							//618:Wire
    MSG_PARA_RANGE,							//619:Range
    MSG_LOWER_LIMIT,						//620:Lower limit
    MSG_UPPER_LIMIT,						//621:Upper limit
	MSG_VISION,								//622
    MSG_LIGHT_PARA_2,						//623:Lighting para
    MSG_WIRE_END_ERR,						//624: Wire End  
    MSG_SURE_TO_DELETE_PROGRAM,				//625: Are you sure to delete all your record?   
    MSG_BLUE_COAX,							//626:Lighting para
    MSG_RED_COAX,							//627:Lighting para
    MSG_BLUE_SIDE,							//628:Lighting para
    MSG_RED_SIDE,							//629:Lighting para
    MSG_THRESHOLD_2,						//630:Lighting para
    MSG_KEYBOARD_EXPAND,					//631:Expand>>
    MSG_KEYBOARD_CLOSE,						//632:<<Close
	MSG_MHS_2,								//633: material handling system
	MSG_PREHEAT,							//634: Clamp and PreHeat
	MSG_BSD_DEBUG,							//635: BSD debug
	MSG_BSD_DEBUG_INFO_RECORD,				//636: BSD debug info record
	MSG_RECOMMENDED_BSD_PARA,				//637: BSD Recommended Para
	MSG_CONFIRM_NO_ERR,						//638: BSD Confirm
	MSG_APPLY_2,							//639: Apply
	MSG_CLEAR_ALL,							//640: Clear All Tracks
	MSG_AUTO_SAMPLE_ON,						//641: Auto Sampling Enable
	MSG_OK_TO_AUTO_SAMPLE,					//642: Auto Sampling triggered
	MSG_DIE_DETECT,							//643:Die Detect Threshold (V):
    MSG_LEAD_DETECT,						//644:Lead Detect Threshold (V):
    MSG_TAIL_SHORT,							//645:Tail Short Threshold:
    MSG_NONSTICK,							//646:Nonstick Threshold:
	MSG_1ST_POWER,							//647:
	MSG_2ND_POWER,							//648:
	MSG_1ST_FORCE,							//649:
	MSG_2ND_FORCE,							//650:
	MSG_PROGRAM_2,							//651:
	MSG_1ST_BOND_POS,						//652:
	MSG_2ND_BOND_POS,						//653:
	MSG_RECIPE,								//654:Recipe:
	MSG_RESERVE_655,						//655
    MSG_LOOP,								//656:Loop
    MSG_NECK_HT_PREMOVE,					//657:Neck Height Pretrigger (%)
    MSG_NECK_DIST_PREMOVE,					//658:Neck Distance Pretrigger (%)
    MSG_REV_HT2_PREMOVE,					//659:Reverse Height2 Pretrigger (%)
    MSG_REV_DIST2_PREMOVE,					//660:Reverse Distance2 Pretrigger (%)
    MSG_LOOP_TOP_PREMOVE,					//661:Loop Top Pretrigger (%)
    MSG_PR_VERSION,							//662:Vision Version:
	MSG_NO_ELEC_POWER,						//663:UPS Auto shut down:
	MSG_LAST_LF,							//664
    MSG_1ST_BOND_BOOST,						//665:1st Bond Boost
    MSG_BOOST_DURATION,						//666:Duration
    MSG_BOOST_MODE,							//667:Mode
    MSG_BOOST_AMPLITUDE,					//668:Amplitude
    MSG_SHORT,								//669:Short
    MSG_LONG,								//670:Long
    MSG_FREQUENCY,							//671:Freqency
	MSG_IMPEDANCE,							//672:Impedance
	MSG_TRANSDUCER,							//673:Transducer
    MSG_POWER_SEAT,							//674:Seat Power (Dac/ms)
	MSG_POWER_FEED,							//675:Feed Power (Dac/ms)
	MSG_POWER_TAIL,							//676:Tail Power (Dac/ms)
	MSG_TIME_SEAT,							//677:Seat Time
	MSG_TIME_FEED,							//678:Feed Time
	MSG_TIME_TAIL,							//679:Tail Time
	MSG_USG_ERR,							//680:USG Error 
	MSG_TIME,								//681
	MSG_LEVEL,								//682:Level 
	MSG_GENERATE,							//683:Generate 
	MSG_VOLTAGE,							//684:Voltage 
	MSG_CURRENT_2,							//685:Current 
	MSG_POWER,								//686:Power 
	MSG_MODE,								//687:Mode 
	MSG_USG_SETUP,							//688:USG Setup
	MSG_HIGH_FREQ,							//689:High Frequency
	MSG_LOW_FREQ,							//690:Low Frequency
	MSG_USG_OUTPUT_GENERATE,				//691:Generate
	MSG_USG_CALI_SUCCESS,					//692:USG calibration complete!
	MSG_IMPEDANCE_OUT_RANGE,				//693:USG calibration FAILED!
	MSG_USG_CALI_FAIL,						//694	//v3.1.T1 edit
	MSG_FILE,								//695:File Management tool!
	MSG_DIAGNOSTIC_TOOL,					//696:Diagnostic tool!
	MSG_COPPER_SEQ,							//697:Copper Sequence
    MSG_LF_INFO,							//698:Lead Frame Information
    MSG_TOTAL_UNIT,							//699:Total Units
    MSG_INDEX_TIMES,						//700:Index Times
	MSG_ONLOAD_PICKER,						//701
	MSG_INJECTOR_STEPPER,					//702
	MSG_OFFLOAD_KICKER,						//703  
	MSG_MHS_UTILITY,						//704
	MSG_LEFT_CLAW,							//705
	MSG_RIGHT_CLAW,							//706
    MSG_COMPENSATION,						//707: Compensation
	MSG_CLAMPER_2,							//708: Clamper Control
	MSG_SINGLE_INDEX,						//709: Single Index
    MSG_LEAD_PR,							//710: Lead PR:
	MSG_CTACT_LEVEL_SRCH,					//711: Contact level search
	MSG_START_TO_BEGIN_NEW_TEST,			//712: Start Contact level search
	MSG_ONLOAD,								//713: ONLOAD
	MSG_TRACK_FRONT,						//714: TRACK FRONT
	MSG_TRACK_BACK,							//715: TRACK BACK
	MSG_SENSOR_AND_CYLINDER,				//716: sensor page
	MSG_STEPPER,							//717
    MSG_ONLOAD_TRACK_DOUBLE_LF,				//718: Onload Track Double Lead Frame!
	MSG_LF_STANDBY_UNIT,					//719: LF standby Unit:
	MSG_LEFT_CLAW_TO_BOND_AREA,				//720: Left Claw to Bond Area Units: 
	MSG_BOND_AREA_TO_RIGHT_CLAW,			//721: Bond Area to Right Claw Units:
	MSG_PREHEAT_DELAY,						//722: Injector Delay(ms):
	MSG_CLAMP_OPEN_PRETRIG_CLAW,			//723: Clamp Open Pre-trigger Claw (%):
	MSG_CLAW_PRETRIG_CLAMP_CLOSE,			//724: Claw Pre-trigger Clamp Close (%):
	MSG_CATCH_UP_SPEED,						//725: Claw Catch Up:
	MSG_INDEX,								//726: Index:
	MSG_INDEX_SPEED,						//727: Index Speed
	MSG_DOUBLE_CHECK_POS,					//728: Double Check Position:
	MSG_INJECTOR_POS,						//729: Inject Position:
	MSG_CHECK_POS,							//730: CheckPos
	MSG_INJECT,								//731: Inject
	MSG_RUN_WITH_SENSORS,					//732: Run With Sensors
	MSG_MODULE_TEST,						//733: Module Test
	MSG_SENSOR,								//734: Sensor
	MSG_INDEX_CYCLE_TIME,					//735: Index Cycle Time
    MSG_EXPOSURE_TIME_COLON,				//736:Exposure Time (us):
	MSG_FEATURE,							//737:Feature
	MSG_CONST_WIRE_LENGTH,					//738:Constant Wire Length:
	MSG_TABLE,								//739:Table
	MSG_X_LIMIT,							//740:X Limit
	MSG_Y_LIMIT,							//741:Y Limit
	MSG_START_SRCH_LIMIT,					//742:Will Start to Search Table Limit,
	MSG_SURE_BOND_AREA_CLEAR,				//743:Please Make Sure Bond Area Clear!
    MSG_SRCH_LIMIT_FINISH,					//744:Search Table Limit Finish!
    MSG_POS_COMP_IN_PROGRESS,				//745:Position compensation in progress...
    MSG_POS_COMP_SUCCESS,					//746:Position compensation success!
    MSG_POS_COMP_OUT_RANGE,					//747:Position compensation out of range!
    MSG_POS_COMP_FAIL,						//748:Position compensation fail!
	MSG_BOND_TO_MAKE_CAP_MARK,				//749:Press 'bond' to make capillary mark	//v3.0.T454 edit
	MSG_VERIFY_TO_CHECK_CAP_MARK,			//750:Press 'verify' to check capillary mark
	MSG_BOND,								//751:Bond
	MSG_OK,									//752:Correct
    MSG_OK_TO_CONTINUE,						//753:Press Correct Button to Continue 
	MSG_PR_RETRY,							//754:PR Retry
	MSG_UPDATE_CTACT_LEVEL,					//755:Please Update Contact Level!
    MSG_LIGHT,								//756: Light
	MSG_INDEX_2,							//757: Index  
	MSG_ADJUST,								//758: Adjust
	MSG_THREAD,								//759: Thread   
	MSG_OSCILLATE,							//760: Oscillate
	MSG_ZERO_BTOS,							//761:Zero Btos:
	MSG_AFP_MODE,							//762: AFP Mode
	MSG_IO,									//763: IO
	MSG_ENCODER,							//764: Encoder
	MSG_AUTO_TUNE,							//765: AutoTune
	MSG_FORK_SWITCH_BUFFER,					//766
	MSG_REMOVE_WINDOW_CLAMP,				//767: Change Window Clamper
	MSG_PR_FAIL_2,							//768£ºPR Fail£¡
	MSG_MHS_CALI,							//769£ºMaterial Handler Calibration
	MSG_PR_INDEX_2,							//770£ºPR Index
	MSG_DEVICE,								//771£ºDEVICE PARA
	MSG_SETTING_2,							//772£ºSetting
    MSG_BOND_UNIT,							//773:Bond Unit
	MSG_SENSOR_TO_BOND_AREA,				//774:Sensor to Bond Area:
	MSG_FAB_TEST,							//775:FAB Test:
	MSG_THERMAL_PROTECT_TIME,				//776:Thermal Protection Time
    MSG_CORRECT_BOND_OFFSET,				//777:Correct Bond Offset
	MSG_POWER_COMPENSATION,					//778:Power Compensation
    MSG_CTACT_LEVEL_OFFSET,					//779:Contact Level Offset
    MSG_CIRCLE_SIZE,						//780:Circle Size
	MSG_BUMP_BALL_CTRL,						//781:Bump Ball Control:
    MSG_FILE_NAME_TOO_LONG,					//782:File Name Too Long!
	MSG_INDEXER,							//783: Indexer
	MSG_TEST,								//784: Test
	MSG_DATABASE,							//785: Database
	MSG_MAGAZINE,							//786: Magazine
	MSG_LEADFRAME,							//787: Leadframe
	MSG_PITCH,								//788: Pitch
	MSG_BOTTOM_TO_1ST_SLOT,					//789: Bottom to 1st Slot
	MSG_LAST_SLOT_TO_TOP,					//790: Last Slot to Top
	MSG_EDGE_LENGTH,						//791:
	MSG_CLAMP_COL,							//792: Clamp Columns
	MSG_1ST_HEAT_DELAY,						//793: 1st Heat Delay
	MSG_HEAT_DELAY,							//794: Heat Delay
	MSG_PR_INDEX,							//795: PR Index
	MSG_PR_INDEX_TOL,						//796: PR Index Tol
	MSG_SRCH_BACK_LEN,						//797: Srch Back Length
	MSG_STANDBY_LEN,						//798: Standby Length
	MSG_EJECT_SPEED,						//799: Eject Speed
	MSG_RIGHT_CLEARANCE,					//800: Right Limit Tol
	MSG_SLOWEST,							//801 Slowest
	MSG_SLOWER,								//802: Slower
	MSG_SLOW,								//803: Slow
	MSG_MEDIUM,								//804: Medium
	MSG_FAST,								//805: Fast
	MSG_FASTER,								//806: Faster
	MSG_FASTEST,							//807: Fastest
	MSG_PRECLAMP_POS,						//808: Preclamp Position:
	MSG_PRECLAMP,							//809: Preclamp
	MSG_REMOVE,								//810: Remove
	MSG_ONLOAD_1ST_SLOT,					//811: Onload 1st Slot:
	MSG_SLOT_PITCH_2,						//812: Slot Pitch:
	MSG_FINE_TUNE_SLOT_POS,					//813:
	MSG_SWAP_PITCH,							//814: Swap Pitch:
	MSG_NEXT_SLOT,							//815: Next Slot
	MSG_INDEXER_ALIGN,						//816: Indexer Alignment
	MSG_INDEXER_RIGHT_LIMIT,				//817: Indexer Right Limit:
	MSG_INDEXER_LOGIC_ZERO,					//818: Indexer Logic Zero:
	MSG_INDEX_TUNE,							//819: Index Tune
	MSG_HOLDING_FORK,						//820: Holding Fork
	MSG_PULLY_MOTOR,						//821: Pully Motor
	MSG_STANDBY_POS,						//822
	MSG_LEFT_CLAW_COLON,					//823: left Claw
	MSG_RIGHT_CLAW_COLON,					//824: right Claw
	MSG_PULLER,								//825: Puller
	MSG_ONLOAD_AIR,							//826: Onload Air
	MSG_OFFLOAD_AIR,						//827: Offload Air
	MSG_WITHOUT_LF,							//828: Without Lf
	MSG_SOLENOID_TEST,						//829: Solenoid Test
	MSG_REMOVE_ALL_MAG,						//830: Please Remove all the Magazine On Elevator!
	MSG_WAIT_FORK_STANDBY,					//831: Please Wait Elevator Home and Standby Finish!
    MSG_ADJUST_1ST_SLOT,					//832: Please Adjust First Slot Level! 
	MSG_ADJUST_LAST_SLOT,					//833: Please Adjust Last Slot Level!
	MSG_ADJUST_NEXT_1ST,					//834: Please Adjust the 1st Slot Level Of Next Magazine!
	MSG_SIDE_ONLOADER,						//835: Operating Side: Onloader
	MSG_SIDE_OFFLOADER,						//836: Operating Side: Offloader
	MSG_CONFIRM_TO_LOAD_RECIPE,				//837: Please load recipe		//V1.1.W141 edit
	MSG_DONE_TO_QUIT,						//838: Press 'Done' to quit		//v3.0.T454 edit
	MSG_WAIT_INDEXER_HOME,					//839
	MSG_MOVE_INDEXER_TO_ZERO,				//840: Please Move Indexer to Logic Zero Position!
	MSG_NEXT_TO_SRCH_RIGHT_LIMIT,			//841: Press Next to Search the Right Limit Sensor!
	MSG_NEXT_TO_ADJ_INDEX_OFFSET,			//842: Press Next to Enter Indexing Offset Adjustment!
	MSG_ENSURE_NO_LF_ON_TRACK,				//843: Please Make Sure No Leadframe On the Track!
	MSG_NEXT_TO_INDEX_AND_TUNE,				//844
    MSG_ONLOAD_ELEV_NO_MAG,					//845: Onload Elevator No Magazine!
	MSG_OFFLOAD_ELEV_NO_MAG,				//846: Offload Elevator No Magazine!
	MSG_SRCH_TIME_OUT,						//847: Injection Error!
	MSG_EJECT_WRONG,						//848: Leadframe Jamed the Offload Elevator!
	MSG_ONLOAD_ELEV_NO_RESPONSE,			//849: Onload Elevator No Response!
    MSG_CALIBRATION,						//850: Calibration
    MSG_VERIFICATION,						//851: Verification
    MSG_TAIL_BREAK,							//852: Tail Break
    MSG_DISTANCE,							//853: Distance (um)
	MSG_EJECT,								//854: Eject
	MSG_EDIT_MHS_PROGRAM,					//855£ºEdit Material Handler Program
	MSG_CLAW_OFFSET,						//856: Claw Offset
	MSG_DELETE_ELEV_DATA,					//857: Delete all elevator Data and Re-teach? 
	MSG_DELETE_PR_INDEX_DATA,				//858: Delete all PR Index Data and Re-teach? 
	MSG_REMOVE_LF_ON_TRACK,					//859: Please Manully Remove the Leadframe On Track! 
	MSG_REMOVE_LF_ON_TRACK_AUTOMATICALLY,	//860: AutoMatically Remove the Leadframe On Track! 
	MSG_OPEN_CLAMP_DELAY,					//861: Open Clamp Delay 
	MSG_LF_LEFT_LAST_TIME,					//862: Leadframe left on the track last time!
	MSG_CONTINUE_TO_BOND,                   //863: Do you want to continue or skip to index new one?
    MSG_ENHANCE,							//864: Enhance
	MSG_NO_LF_RUN,							//865: No Lf Run
	MSG_TEACH_MHS_PROGRAM,					//866: Teach Material Handler Program
	MSG_MOVE_TO_SAFE_POS,					//867: Move Table to Safe Position, Then Press Next!
    MSG_TCH_SAFE_POS,						//868: Teach Table Safe Position
	MSG_PGM_EXIST,							//869: Program Exist
	MSG_NO_RECIPE,							//870
	MSG_TCH_NEW,							//871: Teach New 
	MSG_TCH_BOND_PROGRAM,					//872
	MSG_EDIT_BOND_PROGRAM,					//873
    MSG_1ST_LF_PREHEAT_TIME,				//874: 1st LF Preheat Time (ms):
	MSG_NORMAL_LF_PREHEAT_TIME,				//875: Normal LF Preheat Time (ms):
    MSG_OFFLOAD_POS,						//876:Offload Position (1/2):
    MSG_POS1,								//877:Position1
    MSG_POS2,								//878:Position2
	MSG_ADVANCED_VISION,					//879
    MSG_ROTATE_ANGLE,						//880:Rotate Angle:
	MSG_SHAPE_RECOGNIZE,					//881:Shape Recognize
	MSG_1ST_IMPACT_FORCE,					//882:Impact Force 1 
	MSG_1ST_IMPACT_TIME,					//883:Impact Time 1 
	MSG_2ND_IMPACT_TIME,					//884:Impact Time 2 
	MSG_1ST_IMPACT_POWER,					//885:Impact Power 1 
	MSG_2ND_IMPACT_POWER,					//886:Impact Power 2 
	MSG_STANDBY_MODE,						//887:Machine is in Standby Mode...
	MSG_QUIT_STANDBY_MODE,					//888:Click Mouse Left Button to Quit Sleep Mode!
    MSG_ENTER_STANDBY_MODE,					//889:Enter Machine Standby Mode
	MSG_BACKUP_ID,							//890:Back-up ID:
	MSG_ARRANGE_BACKUP_PR_AREA,				//891: teach back-up PR 
	MSG_NEXT_TO_FINISH,						//892: Press Next Button to finish! 
	MSG_NEXT_TO_ADJ_LF_POS,					//893: Press Next Button to adjust the Lf position! 
	MSG_SETUP_JIG,							//894: Please setup the Jig and press Z to its surface 
	MSG_NEXT_TO_RECORD_FLAT_LEVEL,			//895: Press Next to record the value! 
	MSG_REMOVE_JIG,							//896: Please Release the bond head then Remove the Jig.
	MSG_FLAT_LEVEL,							//897: Flat Level:
	MSG_MULTI_BTOS,							//898: Multiple BTOS
	MSG_CONFIRM_RED_BUTTON_ON,				//899: Confirm Red Button on the panel is switched ON !
	MSG_ULTRA_LOW,							//900: Ultra Low	//V1.1.W149 edit
    MSG_PREIMPACT_TIME,						//901:Pre Impact Time (ms)
    MSG_PRE_SHIFT_2,						//902:Pre Shift 2 (um)
	MSG_SURE_TO_OVERWRITE_DEVICE,			//903: Are you sure to overwrite current device setting?
	MSG_ENSURE_VISION_NOT_EXPIRED,			//904: Please make sure the vision is not expired
    MSG_DEBUG_2,							//905: Debug
	MSG_N2H2_ENABLE,						//906: N2H2 Enable:
	MSG_N2H2_STABLE_TIME,					//907: N2H2 Stable Time:
	MSG_WAIT_N2H2_STABLE,					//908: Waiting N2H2 Stable
	MSG_ONLOAD_MAG_PULLY_FULL,				//909: Onload Magazine Buffer Full
	MSG_OFFLOAD_MAG_PULLY_FULL,				//910: Offload Magazine Buffer Full  
	MSG_ETORCH_BURN_IN,						//911: E-Torch Burn In
	MSG_CYCLE_TIME,							//912: Cycle:
	MSG_CYCLE_NUM,							//913: Times:
	MSG_SURE_TO_DELETE_BACKUP_PR,			//914: Are you sure to delete all back-up PR?
	MSG_SURE_TO_SWAP_BACKUP_PR,				//915: Are you sure to swap back-up PR to main PR?
    MSG_SWAP,								//916: Swap
	MSG_DELETE_ALL,							//917: Delete All
	MSG_AUTO_SWAP_MAIN_PR,					//918: Auto Swap Main PR
	MSG_MULTI_PR,							//919
	MSG_ENABLE_LEAD_BTOS,					//920: Enable Adjusting Lead BTOS
	MSG_CAP_WEAR_OUT_POWER_COMPENSATE,		//921:Capillary Wear Out Power Compensation
	MSG_START_END_COUNT,					//922:Capillary Count (1/2):
	MSG_POWER_COMPENSATE,					//923:Power Offset (%):
	MSG_FAB2_TAIL_LENGTH,					//924:FAB2 Tail Length:
	MSG_BSOB_FAB_TYPE,						//925	//v3.1.T381 rename
	MSG_HORIZONTAL,							//926:Horizon
	MSG_CIRCLE,								//927:Circle   
	MSG_BALL_SCRUB,							//928:Ball Scrub
	MSG_CONTRAST,							//929:Contrast
	MSG_RED_GAIN,							//930:Red Gain
	MSG_BRIGHTNESS,							//931:Brightness
	MSG_SATURATION,							//932:Saturation
	MSG_PR_FILTER,							//933:PR Filter:
	MSG_CONFIRM_TO_RESET,					//934:Confirm Delete	//V1.1.W261 edit
	MSG_SERIAL_SKIP_NUM,					//935:Serial Skip Num	//V1.1.W262 edit
	MSG_TAIL_BOND,							//936:Tail Bond
    MSG_BOND_TOOLS,							//937:Bond Tools
    MSG_CAP_SMOOTH,							//938:Capi Smooth
    MSG_SMOOTH,								//939:Smooth
    MSG_CLEAN,								//940:Clean
	MSG_START_TO_HOME_INDEXER,				//941:Press "Start" to home the Indexer
	MSG_START_TO_TCH_INDEX_PR,				//942:Press "Start" to teach Index PR
	MSG_TCH_NEW_TO_INDEX_LF,				//943:Press "New" if you want to index a new leadframe
	MSG_DONE_TO_CONFIRM_FINAL_POS,			//944:Press "Done" to confirm final position
	MSG_START_TO_TCH_ELEV_POS,				//945:Press "Start" to teach elevator position
	MSG_NEXT_TO_SKIP,						//946:Press "Next" in bottom to skip this step
	MSG_INCOMPLETE_ROTATE_COMPENSATION,		//947:Incomplete Rolling Compensation:
	MSG_CONTINUE_TO_PREVIEW_BOND_SEQ,		//948:Press 'Continue' to preview the bonding sequence.
    MSG_BBOS_PARA,							//949:Bump On Wedge Parameter
	MSG_HINT_MULTI_DIE,						//950:Hint: If multiple dies in one cup, please input numbers above 
	MSG_HINT_FOUND_MULTI_DIE,				//951:Hint: Found multiple die in one cup
	MSG_CHOOSE_CORRECT_DIE,					//952:Please choose correct die on top combo box.
	MSG_SELF_DEFINE_2,						//953:Bond Self-define
	MSG_INPUT_START_END_WIRE,				//954:Input Bond Start Wire and End Wire Number
	MSG_START_WIRE,							//955:Start Wire:
	MSG_END_WIRE,							//956:End Wire:
    //MSG_BUMP_WIRE_BOND_POWER,				//957:Bump Wire Base Power
    MSG_RESERVE_957,						//957
	MSG_ONLOAD_PUSHER_KICKER,				//958:Onloader Push Kicker
	MSG_OFFLOAD_TIMEOUT,					//959:Offloader Jam
	MSG_SURE_TO_DELETE_RECIPE,				//960:Are You Sure To Delete Original Recipe
	MSG_TCH_NEW_RECIPE,						//961:And Teach New Recipe?
	MSG_EDIT_PT,							//962:Edit Point
	MSG_LOOP_PRETRIG,						//963:Looping Pre Trigger
	MSG_WCL_PRETRIG,						//964:Wire Clamp Pre Trigger
	MSG_TAIL_BREAK_THRES,					//965:Tail Break Threshold:
    MSG_1ST_PRE_IMPACT_POWER,				//966:Pre Impact Power 1
    MSG_2ND_PRE_IMPACT_POWER,				//967:Pre Impact Power 2
	MSG_DIRECTION_ANGLE,					//968
    MSG_FLEX,								//969:Flex
    MSG_BUMP_SHAPE_MODE,					//970:Bump Shape Mode
    MSG_REV_MOTION,							//971:Flex Length (um)
    MSG_RESERVE_972,						//972
    MSG_LOOP_HT,							//973:Flex Height (um)
    MSG_STICK_DETECTION,					//974:Stick Detection
	MSG_BH_NOT_IN_SAFE_POS,					//975:BH is not in the safe position
	MSG_WAIT_ON_OFF_LOADER_FINISH,			//976:Wait on/off loader finish...
    MSG_WIRE_FEED_ERR,						//977:No Wire Feed, Check Bond Quality!  
    MSG_FAST_TAIL,							//978:Tail Control
    MSG_WIRE_FEED_CHECK,					//979:Wire Feed Check
    MSG_L_ANGLE,							//980:L Angle
	MSG_INDEX_PR_DELAY,						//981:Index PR Delay
    MSG_TAIL_LEN_MONITOR,					//982:Tail Length Monitor
    MSG_COARSE_DETECT,						//983:Coarse Detection:
    MSG_COARSE_DETECT_SAMPLE,				//984:Coarse Detection Samples:
    MSG_MAT_DIE_AHEAD,						//985:Matrix Die Ahead
    MSG_MAT_ROW_COL,						//986:Matrix Row/Col:
    MSG_MOVE_TO_2ND_COL_TOP_RIGHT_ALIGN_PT,	//987:Move table to matrix top-right 2nd column alignment point.
    MSG_MOVE_TO_2ND_ROW_TOP_RIGHT_ALIGN_PT,	//988:Move table to matrix top-right 2nd row alignment point.
    MSG_UNIT_ROW_COL_NUM_WRONG,				//989:Unit Row/Column number wrong!
    MSG_MAT_ROW_COL_NUM_WRONG,				//990:Matrix Row/Column number wrong!
    MSG_TCH_MAT_PITCH,						//991:Teach Matrix Pitch:
    MSG_POWER_2,							//992:Power
	MSG_TIME_2,								//993:Time
    MSG_CTACT_POWER_PERCENT,				//994
    MSG_BOND_POWER_PERCENT,					//995
    MSG_RELEASE_POWER_PERCENT,				//996
    MSG_TCH_PAD_SIZE,						//997:Teach Pad Size
    MSG_STEP1,								//998:Step1:
    MSG_MOVE_TO_TOP_LEFT_PAD_PT,			//999:Move cross hair to left-up point of pad.
    MSG_ENTER_TO_CONTINUE,					//1000:Press 'ENTER' to continue.
    MSG_ESC_TO_CANCEL,						//1001:Press 'ESC' to cancel.
    MSG_ESCAPE,								//1002:ESC
    MSG_ENTER,								//1003:Enter
    MSG_STEP2,								//1004:Step2:
    MSG_STEP3,								//1005:Step3:
    MSG_MOVE_TO_TOP_RIGHT_PAD_PT,			//1006:Move cross hair to Right-Up point of pad.
    MSG_MOVE_TO_BOTTOM_RIGHT_PAD_PT,		//1007:Move cross hair to Right-Bottom point of pad.
    MSG_EDIT_PAD_SIZE,						//1008:Edit Pad Size
    MSG_STEP4,								//1009:Step4:
    MSG_TCH_BOND_PT,						//1010:Teach Bond Points
    MSG_MOVE_TO_GROUP_1ST_PT,				//1011:Move To Group 1st Point
    MSG_MOVE_TO_GROUP_2ND_PT,				//1012:Move To Group 2nd Point
    MSG_MOVE_TO_GROUP_LAST_PT,				//1013:Move To Group Last Point
    MSG_WIRE_LEN_CTRL,						//1014:Wire Length Control
    MSG_CONST_WIRE_LEN,						//1015:Constant Wire Length
    MSG_WIRE_LEN,							//1016:Wire Length
    MSG_0_NO_1_YES,							//1017:
    MSG_LENGTH,								//1018:Length:
    MSG_NO_LF_BTW_TRACK_ELEV,				//1019:Confirm no LF between track and elevators.
    MSG_NO_MAG_IN_ELEV,						//1020:Confirm no magazine under elevators.
    MSG_PAD_PATTERN_MGR,					//1021:Pad Pattern Manager
    MSG_PAD_PATTERN,						//1022:Pad Patterns
    MSG_CURRENT_3,							//1023:Current
    MSG_UPH_MODE,							//1024:UPH Mode
    MSG_REF,								//1025:Bond On
    MSG_PAD_TYPE,							//1026:Pad Type
    MSG_TCH_BOND_PT_2,						//1027:Teach Bond Point
    MSG_TAUGHT_BOND_PT,						//1028:Taught(1/2)
    MSG_SINGLE,								//1029:Single
    MSG_GROUP,								//1030:Group
    MSG_INTELLIGENT_PAD_LOCATOR,			//1031:Intelli Pad Locator
    MSG_BOND_ON_PROPERTY,					//1032:Bond On Property
    MSG_1ST_BOND_PT_PAD_ID,					//1033:1st Bond Point Pad ID
    MSG_1ST_BOND_PT_X_OFFSET,				//1034:1st Bond Point X Offset
    MSG_1ST_BOND_PT_Y_OFFSET,				//1035:1st Bond Point Y Offset
    MSG_PAD_LOCATE_FAIL,					//1036:Pad locating failure!
    MSG_MANUAL_LOCATE_PAD,					//1037:Please manually locate pad.
    MSG_PAD_LOCATE_FAIL_SELECT_PAD_TYPE,	//1038:Pad locating fail! Pls select pad type.
    MSG_THEN_MANUAL_LOCATE_PAD,				//1039:Then manually locate pad.
    MSG_BOND_PT_NOT_MATCH,					//1040:1st and 2nd bond point are not match.
	MSG_CAPILLARY,							//1041:Capillary
    MSG_CHANGE_CAP_2,						//1042:Change Capillary
    MSG_REMOVE_OLD_CAP,						//1043:Remove Old Capillary.
    MSG_INSERT_NEW_CAP,						//1044:Insert New Capillary.
    MSG_INSERT_CAP_JIG,						//1045:Insert Capillary Jig Under Transducer.
    MSG_TIGHTEN_CAP,						//1046:Tight Capillary With Proper Torque.
    MSG_REMOVE_CAP_JIG,						//1047:Remove Capillary Jig.
    MSG_CHECK_IMPEDANCE,					//1048:Please Check USG Impedance.
    MSG_STEP5,								//1049:Step5:
    MSG_STEP6,								//1050:Step6:
    MSG_STEP7,								//1051:Step7:
	MSG_POWER_ON,							//1052:Power On
	MSG_POWER_OFF,							//1053:Power Off
    MSG_TCH_DUMMY_BOND_AREA,				//1054:Teach Dummy Bond Area
    MSG_MOVE_TO_TOP_LEFT_PT,				//1055:Move cross hair to left-top point.
    MSG_MOVE_TO_BOTTOM_RIGHT_PT,			//1056:Move cross hair to right-bottom point.
    MSG_CONTINUE_TO_LEARN_CTACT_LEVEL,		//1057:Press "Continue" to learn contact level.
    MSG_TEACH,								//1058:Teach
    MSG_BOND_AREA,							//1059:Bond Area
    MSG_AREA_TEACH_STATUS,					//1060:Area Teach Status
    MSG_SWAP_WIRE,							//1061:Swap Wire
    MSG_FIRST_WIRE,							//1062:First Wire
    MSG_SECOND_WIRE,						//1063:Second Wire
    MSG_WIRE_LEN_CORRECT,					//1064:Wire Length Correction
    MSG_1ST_BOND_PARA,						//1065:1st Bond Parameter
    MSG_2ND_BOND_PARA,						//1066:2nd Bond Parameter
    MSG_IPL_DELAY,							//1067:IPL Delay (ms)
    MSG_PRE_KINK,							//1068:Pre Kink
    MSG_PRE_KINK_HT,						//1069:Pre Kink Height (um)
    MSG_PRE_KINK_DIST,						//1070:Pre Kink Dist (um)
    MSG_PRE_KINK_ANGLE,						//1071:Pre Kink Angle (Deg)
	MSG_1ST_PREHEAT_DELAY,					//1072:1st PreHeat Delay  
    MSG_HARDWARE_TEST,						//1073:Hardware Test
    MSG_RESERVE_1074,						//1074
    MSG_CONVERT_TO_SINGLE_SEQ,				//1075:Please Convert To Single Unit First!
    MSG_SINGLE_UNIT,						//1076:Single Unit
    MSG_GROUPING,							//1077:Grouping
    MSG_APPLY_TO,							//1078:Apply To:
    MSG_VACUUM_CONTROL,						//1079:Vacuum Control
    MSG_VACUUM_PRESSURE_LOW,				//1080:Vacuum Pressure Too Low!
    MSG_VACUUM,								//1081:Vacuum:
    MSG_SELECT_PROGRAM_TYPE,				//1082:Please Select Desired Program Type.
    MSG_2ND_BOND_PT_PAD_ID,					//1083:2nd Bond Point Pad ID
    MSG_2ND_BOND_PT_X_OFFSET,				//1084:2nd Bond Point X Offset
    MSG_2ND_BOND_PT_Y_OFFSET,				//1085:2nd Bond Point Y Offset
    MSG_1ST_BOND_SCRUB,						//1086:1st Bond Scrub
    MSG_2ND_BOND_SCRUB,						//1087:2nd Bond Scrub
    MSG_BUMP_BALL_SCRUB,					//1088:Bump Ball Scrub
    MSG_IF_CHANGE_PROGRAM_TYPE,				//1089:If Change To Different Program Type,
    MSG_THEN_EDIT_MANUAL_AND_PR_ALIGN,		//1090:Please Quit Current Menu, Edit Manual And PR Alignment.
    MSG_IF_NOT_CHANGE_PROGRAM_TYPE,			//1091:If No Change Of Program Type,
    MSG_X_MOTOR_OVERHEAT,					//1092:X Motor Over Heat.
    MSG_Y_MOTOR_OVERHEAT,					//1093:Y Motor Over Heat.
    MSG_EFO_TEST,							//1094:EFO
	MSG_SPARK_CONFIG,						//1095:Spark Config
	MSG_GAP_LARGE_DAC,						//1096:Gap Large DAC
    MSG_LENGTH_2,							//1097:Length
    MSG_CTACT_DIFF,							//1098:CD
    MSG_PROFILE,							//1099:Profile
    MSG_1ST_KINK,							//1100:1st Kink
    MSG_2ND_KINK,							//1101:2nd Kink
    MSG_3RD_KINK,							//1102:3rd Kink
    MSG_LAST_KINK,							//1103:Last Kink
    MSG_LANDING,							//1104:Landing
	MSG_KINK3_SPAN_LEN,						//1105	//v3.1.T145 rename
	MSG_KINK3_SPAN_LEN_UNIT,				//1106	//v3.1.T145 rename
	MSG_KINK3_HT_FACTOR,					//1107:Kink3 Height Factor
	MSG_KINK3_SMOOTH,						//1108:Kink3 Smoothness
	MSG_SLOPE_LEN,							//1109:Slope Length (um)
	MSG_CTACT_ANGLE_CORRECT,				//1110:Contact Angle Correct (Degree)
	MSG_TWIST_RATIO,						//1111:Twist Ratio (%)
	MSG_SURE_TO_CHANGE_LOOP_PROFILE,		//1112:Are you sure to change loop profile?
	MSG_SURE_TO_CHANGE_LOOP_TYPE,			//1113:Are you sure to change loop type?
	MSG_KINK1_WIRE_FEED_SPEED,				//1114:Kink1 Wire Feed Speed (%)
	MSG_KINK1_WIRE_BENT_SPEED,				//1115:Kink1 Wire Bent Speed (%)
	MSG_PRE_KINK_WIRE_FEED_SPEED,			//1116:Pre Kink Wire Feed Speed (%)
	MSG_PRE_KINK_WIRE_BENT_SPEED,			//1117:Pre Kink Wire Bent Speed (%)
	MSG_KINK2_WIRE_FEED_SPEED,				//1118:Kink2 Wire Feed Speed (%)
	MSG_KINK2_WIRE_BENT_SPEED,				//1119:Kink2 Wire Bent Speed (%)
	MSG_KINK3_WIRE_FEED_SPEED,				//1120:Kink3 Wire Feed Speed (%)
	MSG_KINK3_WIRE_BENT_SPEED,				//1121:Kink3 Wire Bent Speed (%)
	MSG_KINK4_WIRE_FEED_SPEED,				//1122:Last Kink Wire Feed Speed (%)
	MSG_TWIST_REV_BENT_SPEED,				//1123:Twist Reverse Bent Speed (%)
	MSG_TWIST_FWD_BENT_SPEED,				//1124:Twist Forward Bent Speed (%)
	MSG_TRAJ_SPEED,							//1125:Trajectory Speed (%)
	MSG_PULL_SPEED,							//1126:Pull Speed (%)
    MSG_REV_HT3_PREMOVE,					//1127:Reverse Height3 Pretrigger (%)
    MSG_REV_DIST3_PREMOVE,					//1128:Reverse Distance3 Pretrigger (%)
    MSG_REV_HT4_PREMOVE,					//1129:Reverse Height4 Pretrigger (%)
    MSG_TWIST_PREMOVE,						//1130:Twist Pretrigger (%)
	MSG_REMOVE_MAG_ON_ELEV,					//1131:Remove Magazines from the Elevator
	MSG_REMOVE_MAG_BELOW_ELEV,				//1132:Remove Magazines from the Elevator
    MSG_LEAD_TIP_OFFSET,					//1133:Lead Tip Offset (um)
    MSG_ILL_DELAY,							//1134:ILL Delay (ms)
    MSG_LEAD_CAP_OFFSET,					//1135:Lead Capillary Offset (um)
    MSG_INTELLIGENT_LEAD_LOCATOR,			//1136:Intelli Lead Locator
    MSG_1ST_BOND_PT_ILL,					//1137:1st Bond Point ILL
    MSG_1ST_BOND_PT_LEAD_CAP_OFFSET,		//1138:1st Bond Point Lead Capillary Offset
    MSG_1ST_BOND_PT_LEAD_X_OFFSET,			//1139:1st Bond Point Lead X Offset
    MSG_1ST_BOND_PT_LEAD_Y_OFFSET,			//1140:1st Bond Point Lead Y Offset
    MSG_2ND_BOND_PT_ILL,					//1141:2nd Bond Point ILL
    MSG_2ND_BOND_PT_LEAD_CAP_OFFSET,		//1142:2nd Bond Point Lead Capillary Offset
    MSG_2ND_BOND_PT_LEAD_X_OFFSET,			//1143:2nd Bond Point Lead X Offset
    MSG_2ND_BOND_PT_LEAD_Y_OFFSET,			//1144:2nd Bond Point Lead Y Offset
	MSG_CUTTING_POWER,						//1145
	MSG_CUTTING_POWER_DELAY,				//1146
    MSG_MAT_HYBRID,							//1147:Matrix Hybrid
    MSG_LEAD_LOCATE_FAILURE,				//1148:Lead locating failure!
    MSG_MANUAL_LOCATE_LEAD,					//1149:Please manually locate lead.
    MSG_AUTO,								//1150:Auto
    MSG_SPARK_OPEN,							//1151:Spark Open!
	MSG_SPARK_GAP_LARGE,					//1152:Spark Gap Large!
	MSG_1ST_UNIT_HEAT_DELAY,				//1153:1st Unit Heat Delay
    MSG_CALI_IN_PROGRESS,					//1154:Calibration In Progress...
	MSG_USG_CALI,							//1155
	MSG_STEPPER_CTRL,						//1156:Stepper Control
	MSG_CAP_AUTO_UNPLUG,					//1157:Capillary Auto Unplug
	MSG_UNPLUG,								//1158:Unplug
    MSG_TCH_DUMMY_BOND_AREA_2,				//1159:Please Teach Dummy Bond Area!
	MSG_CAP_UNPLUG_FAIL,					//1160:Capillary Unplug Failure!
	MSG_THREAD_WIRE,						//1161:Thread Wire
	MSG_LF_HOLDER,							//1162:Lead Frame Holder
	MSG_MAT_INFO,							//1163:Matrix Information
    MSG_MAT_ROW_COLON,						//1164:Matrix Row:
    MSG_MAT_COL_COLON,						//1165:Matrix Column:
    MSG_CUR_MAT,							//1166:Current Matrix:
    MSG_MAT_BOND_SCHEDULE,					//1167:Matrix Bonding Schedule
	MSG_MAT_LAYOUT,							//1168:Matrix Layout
	MSG_INDEX_TIME,							//1169:Index Time
	MSG_TOTAL_ROW,							//1170:Total Row
	MSG_TOTAL_COL,							//1171:Total Column
	MSG_CUR_ROW,							//1172:Current Row
	MSG_CUR_COL,							//1173:Current Column
	MSG_RESCHEDULE,							//1174:Reschedule
    MSG_WIDTH,								//1175:LF width
    MSG_MAG_WIDTH,							//1176:Magazine width
    MSG_MAG_LEN,							//1177:Magazine Length
    MSG_DEVICE_OFFSET,						//1178:Magazine Length
    MSG_FORK_DETECTED,						//1179:fork detected
    MSG_OFFLOAD_MAG_DROP_DETECTED,			//1180:Mag drop
    MSG_ONLOAD_MAG_DROP_DETECTED,			//1181:Mag drop
    MSG_MAG_CLAMPER,						//1182:Mag Clamper
    MSG_MATERIAL_HANDLER,					//1183:Material Handler
    MSG_CLAMP_POS,							//1184
    MSG_ONLOAD_MAG_BUFFER_EMPTY,			//1185:Onload Magazine Buffer empty
	MSG_OFFLOAD_MAG_BUFFER_EMPTY,			//1186:Offload Magazine Buffer empty
    MSG_OFFLOAD_OUTPUT_JAM,					//1187:Offload Output Jam
    MSG_NARROW,								//1188:Lead frame track type one
    MSG_WIDE,								//1189:Lead frame track type two
    MSG_CHECK_REAR_RAIL_TYPE,				//1190:check rear rail type
	MSG_WIRE_FEED_COUNT,					//1191
    MSG_OFFLOAD_FORK_DETECTED,				//1192:offload fork detected
	MSG_BSOB_OFFSET,						//1193
	MSG_BUMP_1ST_PRE_CTACT_FORCE,			//1194
	MSG_BUMP_1ST_PRE_CTACT_TIME,			//1195
	MSG_BUMP_1ST_PRE_CTACT_POWER,			//1196
	MSG_BUMP_1ST_BOND_TIME,					//1197
	MSG_BUMP_2ND_BOND_TIME,					//1198
	MSG_BUMP_1ST_BOND_POWER,				//1199
	MSG_BUMP_2ND_BOND_POWER,				//1200
	MSG_BUMP_1ST_BOND_FORCE,				//1201
	MSG_BUMP_2ND_BOND_FORCE,				//1202
	MSG_BUMP_1ST_CTACT_TIME,				//1203
	MSG_BUMP_2ND_CTACT_TIME,				//1204
	MSG_BUMP_1ST_CTACT_POWER,				//1205
	MSG_BUMP_2ND_CTACT_POWER,				//1206
	MSG_BUMP_1ST_CTACT_FORCE,				//1207
	MSG_BUMP_2ND_CTACT_FORCE,				//1208
	MSG_1ST_BUMP_PARA,						//1209
	MSG_2ND_BUMP_PARA,						//1210
	MSG_WIRE_LEN_ERR,						//1211
    MSG_COL,								//1212
    MSG_REF1,								//1213
    MSG_REF2,								//1214
	MSG_BUMP_SHAPE_PARA,					//1215
    MSG_LIGHT_2,							//1216  LED Lighting
    MSG_CHECK_ONLOAD_Y1_SENSOR_PLATE,		//1217: check the sensor plate, sensor should be actived
    MSG_CHECK_ONLOAD_Y2_SENSOR_PLATE,		//1218: check the sensor plate, sensor should be actived
    MSG_CHECK_OFFLOAD_Y1_SENSOR_PLATE,		//1219: check the sensor plate, sensor should be actived
    MSG_CHECK_OFFLOAD_Y2_SENSOR_PLATE,		//1220: check the sensor plate, sensor should be actived
    MSG_CHECK_FRONT_RAIL_SENSOR_PLATE,		//1221: check the sensor plate, sensor should be actived
    MSG_CHECK_REAR_RAIL_SENSOR_PLATE,		//1222: check the sensor plate, sensor should be actived
    MSG_CHECK_INJECT_LF,					//1223: leadframe Jamed at the input Elevator!
    MSG_MOTOR_OUT_OF_LIMIT,					//1224: move motor out of the software limit
    MSG_SIDE_CLAMPER,						//1225: magazine side clamper, X
    MSG_FRONT_CLAMPER,						//1226: magazine front clamper, Y2
    MSG_REAR_CLAMPER,						//1227: magazine rear clamper, Y1
    MSG_ACC_RATIO,							//1228: acceleration Ratio //Jerk
	MSG_FRONT_RAIL,							//1229: front rail
	MSG_REAR_RAIL,							//1230: rear rail
	MSG_CUR_DEPTH,							//1231: current depth
    MSG_EJECTOR,							//1232: enjector
    MSG_MOVE_DIST,							//1233: move distance:
    MSG_DIST_TO_HOME,						//1234: distance to HOME:
	MSG_FRONT_REAR_CLAMPER,					//1235: magazine front & rear clamper,
	MSG_INJECTOR_ERR,						//1236:  injector error or no good,
	MSG_SHIFT,								//1237:  shift
	MSG_WIDTH_2,							//1238:  width
	MSG_CHECK_WINDOW_CLAMP_SENSOR_PLATE,	//1239: check the sensor plate, sensor should be actived
	MSG_CLOSE_OTHER_DIALOG,					//1240: close other dialog
	MSG_SAVE,								//1241:  save file
    MSG_MACHINE_OFFSET,						//1242:  machine offset
    MSG_INJECT_2,							//1243: inject unit
    MSG_SEARCH,								//1244: search unit
    MSG_COMPENSATE,							//1245: compensate unit
    MSG_CATCH_UP,							//1246: catch up unit
    MSG_HALF_INDEX,							//1247: half index unit
    MSG_SLOT,								//1248: Slot
    MSG_YES,								//1249: yes
    MSG_NO,									//1250: no
	MSG_HEIGHT,								//1251: height
	MSG_SELF_DEFINE,						//1252: Self Define
	MSG_CANT_LOAD_FROM_CUELITE,				//1253: cannot load from CUELITE.ini file
	MSG_LOAD_BY_CALC,						//1254: load setting by caluation
    MSG_SAVE_CUELITE,						//1255: save machine offset to CUELITE.ini file
    MSG_INJECTOR,							//1256: injector
    MSG_CHECK_FORK,							//1257: check forks
    MSG_NEGATIVE_VALUE,						//1258: value is negative, carefully
	MSG_LOGIC_ZERO_TO_HOME,					//1259: Indexer Logic Zero to home
 	MSG_LOGIC_RIGHT_LIMIT_TO_HOME,			//1260: Indexer Logic Right to home
	MSG_STANDBY_LEN_UM,						//1261: Standby Length
	MSG_RIGHT_CLEARANCE_UM,					//1262
	MSG_FRONT_REAR_RAIL,					//1263: Front & Real Rail,
	MSG_REMOVE_WINDOW_CLMAP,				//1264: Please Manully Remove window clamp ! 
	MSG_INSTALL_WINDOW_CLMAP,				//1265: Please Manully Install window clamp ! 
	MSG_REMOVE_HEATER_BLOCK,				//1266: Please Manully Remove heater block ! 
	MSG_INSTALL_HEATER_BLOCK,				//1267: Please Manully Install heater block ! 
    MSG_CANT_FIND_MACHINE_OFFSET_REF_FILE,	//1268: Cannot fine machine offset reference file !
	MSG_ONLOAD_ELEV_HOME_FAIL,				//1269: Input Elevator Home fail !
	MSG_ONLOAD_X_HOME_FAIL,					//1270: Input Side Clamper Home fail!
	MSG_ONLOAD_Y_ONE_HOME_FAIL,				//1271: Input Rear Clamper fail !
	MSG_ONLOAD_Y_TWO_HOME_FAIL,				//1272: Input Front Clamper Home fail!
	MSG_OFFLOAD_ELEV_HOME_FAIL,				//1273: Output Elevator Home fail !
	MSG_OFFLOAD_X_HOME_FAIL,				//1274: Output Side Clamper Home fail!
	MSG_OFFLOAD_Y_ONE_HOME_FAIL,			//1275: Output Rear Clamper fail !
	MSG_OFFLOAD_Y_TWO_HOME_FAIL,			//1276: Output Front Clamper Home fail!
	MSG_FRONT_RAIL_HOME_FAIL,				//1277: Front Rail Home fail !
	MSG_REAR_RAIL_HOME_FAIL,				//1278: Rear Rail Home fail!
	MSG_INDEXER_HOME_FAIL,					//1279: Indexer Home fail !
	MSG_WINDOW_CLAMP_HOME_FAIL,				//1280: Window Clamper Home fail!
	MSG_WAIT_EVENT_TIMOUT,					//1281: Wait Event Timeout !
	MSG_RAIL_TOUCH_HEATER_BLOCK,			//1282: Rail touch to heater block !
	MSG_HEAT_PROTECT_MINS,					//1283: Heat Protectin for Window Clamper, in ms
	MSG_LOAD_MHS_DATA,						//1284: Load Material Handler Data?
	MSG_FAB_TYPE,						    //1285	//v3.1.T388 rename
	MSG_CHECK_BOND_HEAD_AT_SAFE_POS,		//1286: Check bonding head is safe
    MSG_1ST_ONLY,							//1287: 1st unit only
	MSG_AGC,								//1288: camera AGC
    MSG_FORCE_AUTO,							//1289: Force auto calibration
	MSG_YES_TO_PROCEED,						//1290
	MSG_UNIT,								//1291
	MSG_INDEXER_FAIL,						//1292: Indexer fail !
	MSG_EDGE_TO_INDEX_HOLE,					//1293: index hole distance to edge
	MSG_NEW_REEL,							//1294: index AS new reel
	MSG_ONLOAD_DOWN,						//1295: Onload is down
	MSG_OFFLOAD_DOWN,						//1296: Offload is down
    MSG_SKIP_1ST_ONLY,						//1297: skip 1st unit only
    MSG_VACUUM_CHECK,						//1298: Vacuum check
    MSG_INDEX_TIME_TOO_LONG,				//1299: index time > TOP_MAX_INDEX_TIME
	MSG_ONLOAD_ERR,							//1300: Onload is ERROR
	MSG_OFFLOAD_ERR,						//1301: Offload is ERROR
	MSG_ONLOAD_MAG_PULLY_JAM,				//1302: Onload Magazine Pully Jam
	MSG_OFFLOAD_MAG_PULLY_JAM,				//1303: Offload Magazine Pully Jam
	MSG_CLAMP_POS_2,						//1304: Mag Clamp Pos
	MSG_EMERGENCY_PRESSED,					//1305: Emergency Pressed
	MSG_NO_LF_PUSHED_OUT,					//1306: No LF Pushed Out!
	MSG_ELEV_CALI,							//1307:ELEVATOR Set Up
	MSG_NEXT_MAG_HOLD_POS,					//1308
	MSG_CLAMPER_FRONT_POS,					//1309:CLAMPER_FRONT
	MSG_CLAMPER_BACK_POS,					//1310:CLAMPER_BACK
	MSG_CLAMPER_LEN_POS,					//1311:CLAMPER_LENGTH
	MSG_FRONT_TRACK_POS,					//1312:FRONT_TRACK
	MSG_BACK_TRACK_POS,						//1313:BACK_TRACK
	MSG_MAG_LEN_LIMIT1,						//1314:MAG_LEN_LIMIT1
	MSG_MAG_LEN_LIMIT2,						//1315:MAG_LEN_LIMIT2
	MSG_MAG_LEN_LIMIT3,						//1316:MAG_LEN_LIMIT3
	MSG_ONLOAD_MAG_LEN_LIMIT_FAIL,			//1317:ONLOADER_MAG_LEN_LIMIT_FAIL
	MSG_OFFLOAD_MAG_LEN_LIMIT_FAIL,			//1318:OFFLOADER_MAG_LEN_LIMIT_FAIL
	MSG_CENTER_OFFSET,						//1319:DEVICE_CENTER_OFFSET
	MSG_LOAD_BOND_PARA,						//1320:Load Recipe for Bond Para
	MSG_LOAD_MHS_PARA,						//1321:Load Recipe for MH Para
	MSG_PERIOD_OF_VALIDITY,					//1322:Period of validity
	MSG_CTACT_SUPPLIER_TO_UNLOCK,			//1323:Connet to supplier
	MSG_UNLOCK_SUCCESS,						//1324:Unlock Success!
	MSG_UNLOCK_FAIL,						//1325:Unlock Fail!
	MSG_UNLOCK,								//1326:Unlock!
	MSG_PERIOD_OF_VALIDITY_2,				//1327:Period of validity
	MSG_OPEN_TRACK,							//1328:Open Track
	MSG_CLOSE_TRACK,						//1329:Close Track
	MSG_UNPLUG_2,							//1330:Unplug?
	MSG_ONLOAD_TOTAL_SLOT,					//1331:Total Slot Of Onloader:
	MSG_OFFLOAD_TOTAL_SLOT,					//1332:Total Slot Of Offloader:
	MSG_AUTO_SKIP_SCORE,					//1333:Auto Skip Score
	MSG_IS_CONTINUE_AUTO_SKIP,				//1334:Is continue Skip Unit
	MSG_PR_FAIL,							//1335:PR Fail
	MSG_PR_QUALITY_REJECT,					//1336:PR Quality Reject/ Out of range
	MSG_PR_INDEX_SCORE,						//1337:PR Index Score
	MSG_MANUAL_SKIP,						//1338:Manual Skip 
	MSG_FAST_LOOP,							//1339
	MSG_OFFLOAD,							//1340
	MSG_LF_KICK_OUT_POS,					//1341
	MSG_MAG_SUPPORT_POS,					//1342
	MSG_MAG_DROP_POS,						//1343
	MSG_FIRST_LF_POS,						//1344
	MSG_MAG_OFFLOAD_POS,					//1345
	MSG_PITCH_WIDTH,						//1346
	MSG_LF_KICKER,							//1347
	MSG_MAG_ARM_Y,							//1348
	MSG_MAG_ARM_Z,	 						//1349
	MSG_MAG_KICKER,							//1350
	MSG_MAG_ARM_EMPTY,						//1351
	MSG_MAG_DROP_FAIL,						//1352
	MSG_MAG_KICKER_FULL,					//1353
	MSG_MAG_ARM_FULL,						//1354
	MSG_LOAD_MAG,							//1355
	MSG_MOVE_PITCH_FORWARD,					//1356
	MSG_MOVE_PITCH_BACKWARD,				//1357
	MSG_LF_OUTPUT_JAM,						//1358
	MSG_EJECT_MAG,							//1359
	MSG_BOND_FORCE_LIMIT,					//1360
    MSG_1ST_PRE_IMPACT_FORCE,				//1361
    MSG_2ND_PRE_IMPACT_FORCE,				//1362
    MSG_1ST_PRE_IMPACT_TIME,				//1363
    MSG_2ND_PRE_IMPACT_TIME,				//1364
    MSG_SELECT_WIRE,						//1365
    MSG_BACKUP_PR_1,						//1366
    MSG_BACKUP_PR_2,						//1367
    MSG_BACKUP_PR_3,						//1368
    MSG_BACKUP_PR_4,						//1369
    MSG_1ST_BOND_POINT,						//1370
    MSG_2ND_BOND_POINT,						//1371
	MSG_REMOVE_TOP_PLATE_WINDOW_CLAMP,		//1372
	MSG_IS_TOP_PLATE_WINDOW_CLAMP_REMOVED,	//1373
	MSG_CAP_UNPLUG,							//1374
	MSG_SRCH_RANGE_RETRY,					//1375
	MSG_RELAY_SAFETY_DURATION,				//1376
	MSG_OUTPUT_MAG_SLOT_SEQ,				//1377
	MSG_STANDARD_SEQ,						//1378
	MSG_ALTERNATE_SEQ,						//1379
	MSG_INVALID_FILE_NAME,					//1380
	MSG_SPARK_UNKNOWN_ERR,					//1381
	MSG_SPARK_MISSING_RESULT,				//1382
	MSG_NEXT_STEP_PUSHER_IN,				//1383
    MSG_QUIT,								//1384
	MSG_STEP3_2,							//1385
	MSG_SURE_TO_SAVE_DEVICE,				//1386
	MSG_PRECLAMP_CLOSE_DELAY,				//1387
	MSG_SURE_TO_PRECLAMP,					//1388
	MSG_TWIST_FWD_HT_OFFSET,				//1389
	MSG_AUTO_PAD_CENTERING,					//1390
	MSG_REMOVE_ELEV_SUPPORT_BAR,			//1391
	MSG_GAP_FILL,							//1392
	MSG_BINARY_2,							//1393
	MSG_CENTERING_TYPE,						//1394
	MSG_GRAVITY,							//1395
	MSG_CIRCLE_2,							//1396
	MSG_HEAD_TIME_OFFSET,					//1397
	MSG_FULL_CONVERSION,					//1398
	MSG_PARTIAL_CONVERSION,					//1399
	MSG_BLOB_PLUS_EDGE,						//1400
	MSG_EDGE_PLUS_BLOB,						//1401
	MSG_BLOB,								//1402
	MSG_EDGE,								//1403
	MSG_PR_OVERLAP,							//1404
	MSG_DIE_OVERLAP,						//1405
	MSG_HEAT_DELAY_UNIT,					//1406
	MSG_MILLISECOND,						//1407
	MSG_SECOND,								//1408
	MSG_NEXT_LF_WAIT_POS,					//1409
	MSG_REWORK,								//1410
	MSG_CONFIRM_TO_EXIT,					//1411
	MSG_USER_LEVEL,							//1412		//V1.1.W38 add
	MSG_MINIMIZE_WINDOW,					//1413		//V1.1.W51 add
	MSG_CONFIRM_TO_UPDATE,					//1414		//V1.1.W141 add
	MSG_USG_OUTPUT_LEVEL,					//1415		//V1.1.W149 add
	MSG_ELEV_SIDE,							//1416		//V1.1.W149 add
	MSG_CLAMP_DIST,							//1417		//V1.1.W149 add
	MSG_DRIVE_SPEED,						//1418		//V1.1.W149 add
	MSG_VACUUM_DELAY,						//1419		//V1.1.W149 add
	MSG_SKIP_DIE,							//1420		//V1.1.W250 add
	MSG_SKIP_UNIT,							//1421		//V1.1.W250 add
	MSG_RESET_SKIP_STAT,					//1422		//V1.1.W250 add
	MSG_WIRE_LEN_ZERO,						//1423		//V1.1.W254 add
	MSG_MC_NUM,								//1424		//V1.1.W258 add
	MSG_SKIP_PER_INDEX,						//1425		//V1.1.W262 add
	MSG_SKIP_PER_INDEX_EXCEED_LIMIT,		//1426		//V1.1.W265 edit
	MSG_SERIAL_SKIP_LIMIT,					//1427		//V1.1.W265 add
	MSG_FLATTEN_LF_BEFORE_PR_INDEX,			//1428		//V1.4.W35 add
	MSG_LF_FLATTEN_DURATION,				//1429		//V1.4.W35 add
	MSG_EXCEED_POWER_1,						//1430		//v1.5.T4 add
	MSG_EXCEED_POWER_2,						//1431		//v1.5.T7 add
	MSG_WINDOW_CLAMP_NOT_CLOSE,				//1432		//V1.5.W11 add
	MSG_WINDOW_CLAMP_NOT_OPEN,				//1433		//V1.5.W11 add
	MSG_EXCEED_FORCE_1,						//1434
	MSG_EXCEED_FORCE_2,						//1435
	MSG_EXCEED_TIME_1,						//1436
	MSG_EXCEED_TIME_2,						//1437
	MSG_TOTAL_CYCLE,						//1438		//v1.5.T71 add
	MSG_CONFIG_BOND_PROCESS_2,				//1439      //v1.6.T126 add
	MSG_SLOW_SRCH_FAIL,						//1440      //v1.6.P31 add
	MSG_RESET_TO_DEFAULT,					//1441      //v1.6.P196 add
	MSG_LOAD_CONFIG,						//1442      //V1.6.P238 add
	MSG_TIME2,								//1443
	MSG_TIME3,								//1444
	MSG_CURRENT1,							//1445
	MSG_CURRENT2,							//1446
	MSG_CURRENT3,							//1447
	MSG_TIME1,								//1448
	MSG_VERIFY_RECIPE,						//1449
	MSG_1ST_CTACT_LOW,						//1450
	MSG_1ST_CTACT_HIGH,						//1451
	MSG_2ND_CTACT_LOW,						//1452
	MSG_2ND_CTACT_HIGH,						//1453
    MSG_1ST_SRCH_HT,						//1454		//v3.0.T2 add
	MSG_2ND_SRCH_HT,						//1455		//v3.0.T2 add
	MSG_LOAD_MACHINE_CONFIG,				//1456		//v3.0.T137 add
	MSG_INIT_DEVICE_PARA,					//1457		//v3.0.T137 add
	MSG_INIT_STEPPER,						//1458		//v3.0.T137 add
	MSG_INIT_SERVO,							//1459		//v3.0.T137 add
	MSG_INIT_MACHINE_PARA,					//1460		//v3.0.T137 add
	MSG_CHECK_ABNORMAL_EXIT,				//1461		//v3.0.T137 add
	MSG_INIT_CAN_CHANNEL_1,					//1462		//v3.0.T137 add
	MSG_INIT_USG,							//1463		//v3.0.T137 add
	MSG_INITIALIZING,						//1464		//v3.0.T137 add
	MSG_INIT_LIGHTING_COMM,					//1465		//v3.0.T137 add
	MSG_INIT_VISION,						//1466		//v3.0.T137 add
	MSG_LOAD_POWERON_PARA,					//1467		//v3.0.T137 add
	MSG_INIT_BONDER,						//1468		//v3.0.T137 add
	MSG_INIT_EFO,							//1469		//v3.0.T137 add
	MSG_INIT_WIRE_CLAMP,					//1470		//v3.0.T137 add
	MSG_INIT_MHS,							//1471		//v3.0.T137 add
	MSG_INIT_XY_TABLE,						//1472		//v3.0.T137 add
	MSG_LOAD_PR_PATTERN,					//1473		//v3.0.T137 add
	MSG_CHIP_REVERSE,						//1474		//v3.0.L284 add
	MSG_MOVE_TO_LEFT_UNIT2_PT,				//1475		//v3.0.T311 add
	MSG_MOVE_TO_DOWN_UNIT2_PT,				//1476		//v3.0.T311 add
	//v3.0.T433 add
	MSG_BBOS_OFFSET,						//1477
	MSG_BBOS_BASE_HT,						//1478
	MSG_BBOS_SPAN_LEN,						//1479		//v3.1.T383 rename
	MSG_BBOS_BALL_THICKNESS,				//1480
	MSG_BBOS_SCRUB_DIST,					//1481		//v3.1.T383 rename
	MSG_BBOS_CUT_POWER,						//1482		//v3.1.T383 rename
	MSG_BBOS_CUT_POWER_DELAY,				//1483		//v3.1.T383 rename
	MSG_BBOS_MOTION_SPEED,					//1484
	MSG_BBOS_DIRECTION_ANGLE,				//1485
	//v3.0.T434
	MSG_BBOS_BUMP_1ST_PRE_CTACT_TIME,		//1486
	MSG_BBOS_BUMP_1ST_PRE_CTACT_POWER,		//1487
	MSG_BBOS_BUMP_1ST_PRE_CTACT_FORCE,		//1488
	MSG_BBOS_BUMP_1ST_CTACT_TIME,			//1489
	MSG_BBOS_BUMP_2ND_CTACT_TIME,			//1490
	MSG_BBOS_BUMP_1ST_CTACT_POWER,			//1491
	MSG_BBOS_BUMP_2ND_CTACT_POWER,			//1492
	MSG_BBOS_BUMP_1ST_CTACT_FORCE,			//1493
	MSG_BBOS_BUMP_2ND_CTACT_FORCE,			//1494
	MSG_BBOS_BUMP_1ST_BOND_TIME,			//1495
	MSG_BBOS_BUMP_2ND_BOND_TIME,			//1496
	MSG_BBOS_BUMP_1ST_BOND_POWER,			//1497
	MSG_BBOS_BUMP_2ND_BOND_POWER,			//1498
	MSG_BBOS_BUMP_1ST_BOND_FORCE,			//1499
	MSG_BBOS_BUMP_2ND_BOND_FORCE,			//1500
	MSG_AUTO_SRCH,							//1501
	MSG_SELECT_OPER_PRESS_START,			//1502	//v3.0.T453 add
	MSG_INSTALL_INDEX_ALIGN_JIG,			//1503	//v3.0.T453 add
	MSG_PRESS_UP_DOWN_UNTIL_JIG,			//1504	//v3.0.T453 add
	MSG_ALIGN_WITH_BOND_SITE,				//1505	//v3.0.T453 add
    MSG_UP_DOWN_TO_ADJUST_CAP_LEVEL,		//1506	//v3.0.T454 add
    MSG_EDIT_TO_FINE_TUNE_SLOT_POS,			//1507	//v3.0.T455 add
	MSG_ABOVE_LOOP,							//1508	//v3.1.T288 add
	MSG_FAST_LOOP2,							//1509	//v3.1.T288 add
	MSG_BBOS_BUMP_SHAPE_MODE,				//1510	//v3.1.T362 add
	MSG_BBOS_FAB_TYPE,						//1511	//v3.1.T381 add
	MSG_BUMP_WIRE_PARA,						//1512	//v3.1.T401 add
	MSG_BSOB_WIRE_PRE_CTACT_TIME,			//1513	//v3.1.T401 add
	MSG_BSOB_WIRE_PRE_CTACT_FORCE,			//1514	//v3.1.T401 add
	MSG_BSOB_WIRE_PRE_CTACT_POWER,			//1515	//v3.1.T401 add
	MSG_BSOB_WIRE_CTACT_TIME,				//1516	//v3.1.T401 add
	MSG_BSOB_WIRE_CTACT_FORCE,				//1517	//v3.1.T401 add
	MSG_BSOB_WIRE_CTACT_POWER,				//1518	//v3.1.T401 add
	MSG_BSOB_WIRE_BOND_TIME,				//1519	//v3.1.T401 add
	MSG_BSOB_WIRE_BOND_FORCE,				//1520	//v3.1.T401 add
	MSG_BSOB_WIRE_BOND_POWER,				//1521	//v3.1.T401 add
	MSG_BSOB_WIRE_RLS_TIME,					//1522	//v3.1.T401 add
	MSG_BSOB_WIRE_RLS_FORCE,				//1523	//v3.1.T401 add
	MSG_BSOB_WIRE_RLS_POWER,				//1524	//v3.1.T401 add
	MSG_POS_OFFSET,							//1525	//v3.1.T410 add
	MSG_SPAN_LEN_DELAY,						//1526	//v3.1.T410 add
	MSG_ARC_SPAN,							//1527	//v3.1.T410 add
	MSG_ARC_SPAN_DELAY,						//1528	//v3.1.T410 add
	MSG_TOUCHDOWN_FORCE,					//1529	//v3.1.T432 add
	MSG_TOUCHDOWN_FORCE_RANGE,				//1530	//v3.1.T432 add
	MSG_TOUCHDOWN_TIME,						//1531	//v3.1.T432 add
	MSG_GAP_LARGE_TIME,						//1532	//v3.1.T484 add

	APP_MAX_MSG,
};

enum enShowMsgResult
{
    SHOW_MSG_RESULT_OK,
    SHOW_MSG_RESULT_STOP,
    SHOW_MSG_RESULT_SKIP,
};

enum enMsgType
{
	HELP_MSG,
	ERROR_MSG,
	WARNING_MSG
};

#endif
