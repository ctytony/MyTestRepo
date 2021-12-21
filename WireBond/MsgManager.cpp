/***************************************************************
MsgManager.cpp : implementation file
****************************************************************/

#include "StdAfx.h"
#include "MsgManager.h"
#include "MSGDEF.H"
#include "WireBond.h"
#include "KeyPadHandler.h"
#include "HelpMsgDlg.h"
#include "TowerLight.h"
#include "MachineConfig.h"
#include "Bond.h"
#include "DyBtnMsgDlg.h"
#include "WireBondDlg.h"

static WCHAR wcMsgArray[][2][80] = 
{
	_T("����"),													_T("English"),									//0
	_T("Setup"),												_T("У׼"),										//1
	_T("Recipe"),												_T("��ʽ"),										//2
	_T("Run"),													_T("����"),										//3
	_T("Config"),												_T("����"),										//4
	_T("Vision Calibration"),									_T("ͼ��У׼"),									//5
	_T("Shut Down"),											_T("�˳�"),										//6		//V1.1.W71 edit
	_T("Teach Recipe"),											_T("ѧϰ��ʽ"),									//7
	_T("Single Bond"),											_T("��һ����"),									//8
	_T("Auto Bond"),											_T("��������"),									//9
	_T("System Configure"),										_T("ϵͳ�趨"),									//10
	_T("Utility"),												_T("Ӧ��"),										//11
	_T("Debug"),												_T("ϵͳ����"),									//12
	_T("Dummy Recipe"),											_T("�����ʽ"),									//13
	_T("Press Start to teach dummy recipe area"),				_T("��'��ʼ'��ָ�������ʽ����"),				//14
	_T("Move table to top-left position"),						_T("�ƶ�����̨�����Ͻ�"),						//15
	_T("Move table to bottom-right position"),					_T("�ƶ�����̨�����½�"),						//16
	_T("Press 'Next' to continue"),								_T("��'��һ��'����"),							//17	//V1.1.W139 edit
	_T("Teach dummy program completed"),						_T("�����ʽѧϰ���"),							//18
	_T("Recipe Manage"),										_T("��ʽ����"),									//19
	_T("Load Recipe"),											_T("���س�ʽ"),									//20
	_T("Save Recipe"),											_T("�����ʽ"),									//21
	_T("Clear Recipe"),											_T("�����ʽ"),									//22
	_T("Help Message"),											_T("������Ϣ"),									//23
	_T("Error Message"),										_T("������Ϣ"),									//24
	_T("Warning Message"),										_T("������Ϣ"),									//25
	_T("Bond Parameter"),										_T("���Ӳ���"),									//26
	_T("Loop Parameter"),										_T("�߻�����"),									//27
	_T("Ball"),													_T("��"),										//28
	_T("Wedge"),												_T("�ߺ�"),										//29
	_T("Impact Time (ms)"),										_T("ǰ��ʱ�� (ms)"),							//30	//V1.1.W149 edit
	_T("Impact Power (Dac)"),									_T("ǰ������ (Dac)"),							//31	//V1.1.W149 edit
	_T("Impact Force (gm)"),									_T("ǰ��ѹ�� (gm)"),							//32	//V1.1.W149 edit
	_T("Bond Time (ms)"),										_T("����ʱ�� (ms)"),							//33
	_T("Bond Power (Dac)"),										_T("�������� (Dac)"),							//34
	_T("Bond Force (gm)"),										_T("����ѹ�� (gm)"),							//35
	_T("Post Damp Time (ms)"),									_T("����ʱ�� (ms)"),							//36	//V1.1.W149 edit
	_T("Post Damp Power (Dac)"),								_T("�������� (Dac)"),							//37	//V1.1.W149 edit
	_T("Post Damp Force (gm)"),									_T("����ѹ�� (gm)"),							//38	//V1.1.W149 edit
	_T("Power Equalization (Dac)"),								_T("��������ϵ�� (Dac)"),						//39
	_T("Force Equalization (gm)"),								_T("ѹ������ϵ�� (gm)"),						//40
	_T("Done"),													_T("���"),										//41
	_T("Wire Offset"),											_T("���߲���"),									//42	//v3.0.T429 edit
	_T("Loop Group"),											_T("�߻����"),									//43
	_T("Loop Type"),											_T("�߻�����"),									//44
	_T("Loop Height (um)"),										_T("�߻��߶� (um)"),							//45
	_T("Neck Distance (%)"),									_T("�߾����� (%)"),								//46
	_T("Neck Angle (Deg)"),										_T("�߾��Ƕ� (��)"),							//47
	_T("Wire Length Correction (um)"),							_T("�߳����� (um)"),							//48
	_T("Kink2 Span Length"),									_T("�յ�2���"),								//49	//v3.1.T320 edit
	_T("Kink2 Height Factor (%)"),								_T("�յ�2�߶ȱ��� (%)"),						//50	//v3.1.T320 edit
	_T("Pull Distance (um)"),									_T("���쳤�� (um)"),							//51
	_T("Neck Arc Factor (%)"),									_T("�߾����� (%)"),								//52	//v3.1.T407 add
	_T("Loop Base Mode"),										_T("����ģʽ"),									//53
	_T("Kink2 Span Length Unit"),								_T("�յ�2��ȵ�λ"),							//54	//v3.1.T320 edit
	_T("Pre Impact Power (Dac)"),								_T("ǰ��Ԥ������ (Dac)"),						//55
	_T("Neck Arc Delay (%)"),									_T("�߾�������ʱ (%)"),							//56	//v3.1.T407 add
	_T("Bond Setup"),											_T("��������"),									//57
	_T("Reference"),											_T("���յ�"),									//58
	_T("Type"),													_T("����"),										//59
	_T("Contact Level"),										_T("�Ӵ��߶�"),									//60
	_T("Lead"),													_T("�ܽ�"),										//61
	_T("Die"),													_T("оƬ"),										//62
	_T("Gnd"),													_T("����"),										//63
	_T("Move table to contact position"),						_T("�ƶ�����̨���Ӵ�λ��"),						//64
	_T("Contact search successful"),							_T("�Ӵ��������ɹ�"),							//65
	_T("Contact search failure"),								_T("�Ӵ�������ʧ��"),							//66
	_T("New contact level is "),								_T("�½Ӵ��߶��� "),							//67
	_T("Current (XY)"),											_T("��ǰֵ(XY)"),								//68
	_T("New (XY)"),												_T("����ֵ(XY)"),								//69
	_T("Move cursor to capillary mark"),						_T("�ƶ���굽�ɾ�ӡ��"),						//70
	_T("Bond tip offset updated!"),								_T("�ɾ�ƫ����"),								//71
	_T("Lighting Parameters"),									_T("��������"),									//72
	_T("Binary"),												_T("�ڰ�ͼ��"),									//73
	_T("Grey"),													_T("�Ҷ�ͼ��"),									//74
	_T("Cancel"),												_T("ȡ��"),										//75
	_T("Press Start to teach alignment point"),					_T("��'��ʼ'��ָ����λ��"),						//76
	_T("Move table to lead 1st alignment point"),				_T("�ƶ�����̨���ܽŵ�һ��λ��"),				//77
	_T("Move table to lead 2nd alignment point"),				_T("�ƶ�����̨���ܽŵڶ���λ��"),				//78
	_T("Move table to die 1st alignment point"),				_T("�ƶ�����̨��оƬ��һ��λ��"),				//79
	_T("Move table to die 2nd alignment point"),				_T("�ƶ�����̨��оƬ�ڶ���λ��"),				//80
	_T("Adjust lighting for clear image"),						_T("�������������ʾ������ͼ��"),				//81
	_T("Teaching alignment point:"),							_T("ָ����λ��:"),								//82
	_T("Teaching die alignment point"),							_T("ָ��оƬ��λ��"),							//83
	_T("If ready, press Continue"),								_T("׼�����ˣ��밴'����'"),						//84
	_T("Press Next to teach vision pattern"),					_T("��'��һ��'ָ��ͼ��ģ��"),					//85
	_T("Teach alignment point finished"),						_T("ָ����λ�����"),							//86
	_T("Press Start to teach vision pattern"),					_T("��'��ʼ'��ָ��ͼ��ģ��"),					//87
	_T("Teaching vision pattern:"),								_T("ָ��ͼ��ģ��:"),							//88
	_T("Teaching die vision pattern"),							_T("ָ��оƬͼ��ģ��"),							//89
	_T("Move table to lead 1st pattern point"),					_T("�ƶ�����̨���ܽŵ�һģ���"),				//90
	_T("Move table to lead 2nd pattern point"),					_T("�ƶ�����̨���ܽŵڶ�ģ���"),				//91
	_T("Move table to die 1st pattern point"),					_T("�ƶ�����̨��оƬ��һģ���"),				//92
	_T("Move table to die 2nd pattern point"),					_T("�ƶ�����̨��оƬ�ڶ�ģ���"),				//93
	_T("Teach vision pattern finished"),						_T("ָ��ͼ��ģ�����"),							//94
	_T("Press Next to teach wire layout"),						_T("��'��һ��'ָ�����߲���"),					//95
	_T("Move table to wire 1st bond point"),					_T("�ƶ�����̨�����ߵ�һ����"),					//96
	_T("Move table to wire 2nd bond point"),					_T("�ƶ�����̨�����ߵڶ�����"),					//97
	_T("Neck Height Factor 2 (%)"),								_T("�߾��߶ȱ����� (%)"),						//98	//v3.1.T407 add
	_T("Pre Kink Height Factor 2 (%)"),							_T("Ԥ�۵�߶ȱ����� (%)"),						//99	//v3.1.T407 add
	_T("Pre Kink Arc Factor (%)"),								_T("Ԥ�۵㻡�� (%)"),							//100	//v3.1.T407 add
	_T("Pre Kink Arc Delay (%)"),								_T("Ԥ�۵㻡����ʱ (%)"),						//101	//v3.1.T407 add
	_T("Kink2 Height Factor 2 (%)"),							_T("�յ�2�߶ȱ����� (%)"),						//102
	_T("Kink2 Arc Factor (%)"),									_T("�յ�2���� (%)"),							//103
	_T("Kink2 Arc Delay (%)"),									_T("�յ�2������ʱ (%)"),						//104
	_T("Kink3 Height Factor 2 (%)"),							_T("�յ�3�߶ȱ����� (%)"),						//105	//v3.1.T407 add
	_T("Press Start to teach sequence"),						_T("��'��ʼ'��ָ������˳��"),					//106	//v1.5.T91 edit
	_T("Move table to 1st unit alignment point"),				_T("�ƶ�����̨����һ��Ԫ��λ��"),				//107
	_T("Kink3 Arc Factor (%)"),									_T("�յ�3���� (%)"),							//108	//v3.1.T407 add
	_T("Kink3 Arc Delay (%)"),									_T("�յ�3�߶ȱ����� (%)"),						//109	//v3.1.T407 add
	_T("Move table to top-left unit alignment point"),			_T("�ƶ�����̨�����Ͻǵ�Ԫ��λ��"),				//110
	_T("Move table to last unit alignment point"),				_T("�ƶ�����̨�����Ԫ��λ��"),				//111
	_T("Teach sequence finished"),								_T("ָ������˳�����"),							//112	//v1.5.T91 edit
	_T("Kink2 Height (%)"),										_T("�յ�2�߶� (%)"),							//113	//v3.1.T407 add
	_T("Please select Bond Sequence"),							_T("��ѡ�񺸽�˳��"),							//114
	_T("Press Calibrate to learn vision factor"),				_T("��'У׼'��ѧϰͼ��ֱ���ϵ��"),				//115
	_T("Move template to unique pattern point,"),				_T("�ƶ�����̨��һ��ģʽ���صĵ�"),				//116
	_T("Camara rotate angle too large!"),						_T("����ͷ��ת�Ƕ�̫��!"),						//117
	_T("Vision search offset is zero!"),						_T("ͼ��ʶ�����Ϊ0!"),							//118
	_T("Vision factor calibration fail!"),						_T("ͼ��ֱ���ϵ��У׼ʧ��!"),					//119
	_T("Edit Recipe"),											_T("�༭��ʽ"),									//120
	_T("Press Start to edit alignment point"),					_T("��'��ʼ'���༭��λ��"),						//121
	_T("Edit Alignment Point"),									_T("�༭��λ��"),								//122
	_T("Editing alignment point:"),								_T("�༭��λ��:"),								//123
	_T("Kink3 Height (%)"),										_T("�յ�3�߶� (%)"),							//124
	_T("Edit alignment point finished"),						_T("�༭��λ�����"),							//125
	_T("Kink2 Span Length 2"),									_T("�յ�2��ȶ�"),								//126
	_T("Press Next to edit vision pattern"),					_T("��'��һ��'�༭ͼ��ģ��"),					//127
	_T("Editing vision pattern:"),								_T("�༭ͼ��ģ��:"),							//128
	_T("Editing die vision pattern"),							_T("�༭оƬͼ��ģ��"),							//129
	_T("Edit vision pattern finished"),							_T("�༭ͼ��ģ�����"),							//130
	_T("Press Next to edit wire layout"),						_T("��'��һ��'�༭���߲���"),					//131
	_T("Press Start to teach fire level"),						_T("��'��ʼ'ָ������߶�"),						//132
	_T("Press 'Up/Down' to adjust fire level"),					_T("��'����'��������߶�"),						//133	//v3.0.T454 edit
	_T("Or key in fire level"),									_T("����������߶�"),							//134
	_T("Press Confirm to update fire level"),					_T("��'ȷ��'��������߶�"),						//135
	_T("Please do manual alignment"),							_T("�����ֶ���λ"),								//136
	_T("Move table to 1st alignment point"),					_T("�ƶ�����̨����һ��λ��"),					//137
	_T("Move table to 2nd alignment point"),					_T("�ƶ�����̨���ڶ���λ��"),					//138
	_T("Servo System Configure"),								_T("�ŷ�ϵͳ����"),								//139
	_T("Material Handler Parameter"),							_T("����ϵͳ����"),								//140
	_T("Servo System Calibration"),								_T("�ŷ�ϵͳУ׼"),								//141
	_T("Kink3 Span Length 2"),									_T("�յ�3��ȶ�"),								//142
	_T("Neck Span Length 2 (um)"),								_T("�߾���ȶ� (um)"),							//143
	_T("Pre Kink Span Length 2 (um)"),							_T("Ԥ�۵��ȶ� (um)"),						//144
	_T("Force verification success"),							_T("ѹ����֤�ɹ�"),								//145
	_T("Force verification fail"),								_T("ѹ����֤ʧ��"),								//146
	_T("Move table to dummy bond position"),					_T("�ƶ�����̨������λ��"),						//147
	_T("Ensure clear image and proper lighting"),				_T("ȷ��ͼ�������͹�Դ�㹻"),					//148
	_T("Move table to teach track offset"),						_T("�ƶ�����̨ѧϰ���ƫ��"),					//149
	_T("Move cursor to die center"),							_T("�ƶ���굽оƬ����"),						//150
	_T("Press Next to verify bond tip offset"),					_T("��'��һ��'��֤�ɾ�ƫ��"),					//151
	_T("Adjust optics to an unique pattern"),					_T("�ƶ���ͷ��һ����ģ��"),						//152
	_T("Force (gm)"),											_T("ѹ�� (gm)"),								//153
	_T("Wire Clamp"),											_T("�߼�"),										//154
	_T("Contact Search"),										_T("�Ӵ�������"),								//155
	_T("Bond Tip Offset"),										_T("�ɾ�ƫ��"),									//156
	_T("Fire Level"),											_T("����߶�"),									//157
	_T("Bond"),													_T("����"),										//158
	_T("Dummy Bond"),											_T("��β"),										//159
	_T("General"),												_T("����"),										//160
	_T("Statistics"),											_T("ͳ��"),										//161
	_T("Acceleration"),											_T("���ٶ�"),									//162
	_T("Window Clamper"),										_T("֧�ܼо�"),									//163
	_T("Input "),												_T("����"),										//164
	_T("Track"),												_T("����"),										//165
	_T("Output "),												_T("����"),										//166
	_T("Next"),													_T("��һ��"),									//167
	_T("Prev"),													_T("��һ��"),									//168
	_T("Stop Bond"),											_T("ͣ��"),										//169
	_T("BTO"),													_T("�����ɾ�"),									//170	//V1.1.W149 edit
	_T("Correct Bond"),											_T("��һ����"),									//171	//V1.1.W149 edit
	_T("Ball Detect"),											_T("������"),									//172
	_T("Auto Index"),											_T("�Զ�����"),									//173	//V1.1.W149 edit
	_T("Dry Run"),												_T("����"),										//174
	_T("No Vision"),											_T("�޶�λ"),									//175
	_T("Complete"),												_T("�Զ�ͣ��"),									//176
	_T("Bond Point"),											_T("����λ��"),									//177	//V1.1.W149 edit
	_T("Current Wire #"),										_T("��ǰ�ߺ� #"),								//178
	_T("Time Parameters"),										_T("ʱ�����"),									//179
	_T("Stopper Motion:"),										_T("�ƶ�������:"),								//180
	_T("Picker Motion:"),										_T("��е�ֶ���:"),								//181
	_T("Picker Delay:"),										_T("��е����ʱ:"),								//182
	_T("Stopper"),												_T("�ƶ���"),									//183
	_T("Picker"),												_T("��е��"),									//184
	_T("Forward"),												_T("ǰ���˶�"),									//185
	_T("Backward"),												_T("�����˶�"),									//186
	_T("On"),													_T("��"),										//187
	_T("Off"),													_T("��"),										//188
	_T("Inner"),												_T("�ڲർ��"),									//189
	_T("Outer"),												_T("��ർ��"),									//190
	_T("Clamper"),												_T("�о�"),										//191
	_T("Track:"),												_T("����:"),									//192
	_T("Parameters"),											_T("����"),										//193
	_T("Fine Move Speed:"),										_T("��ȷ�˶��ٶ�:"),							//194
	_T("Rough Move Speed:"),									_T("�����˶��ٶ�:"),							//195
	_T("Clamp Position:"),										_T("�н�λ��:"),								//196
	_T("Open Position:"),										_T("�ɿ�λ��:"),								//197
	_T("Home"),													_T("�ع����"),									//198
	_T("Close"),												_T("�н�"),										//199
	_T("Open"),													_T("�ɿ�"),										//200
	_T("Vision System Configure"),								_T("�Ӿ�ϵͳ����"),								//201
	_T("Motion Delay"),											_T("�˶���ʱ"),									//202
	_T("Short Distance (ms):"),									_T("�̾���(ms):"),								//203
	_T("Long Distance (ms):"),									_T("������(ms):"),								//204
	_T("Search Mode"),											_T("ʶ��ʽ"),									//205
	_T("Single Lead PR:"),										_T("�ܽŵ���ʶ��:"),							//206
	_T("Pre Move:"),											_T("��ǰ�ƶ�:"),								//207
	_T("Disable"),												_T("�ϵ�"),										//208
	_T("Enable"),												_T("�ϵ�"),										//209
	_T("Action"),												_T("����"),										//210
	_T("Module:"),												_T("����:"),									//211
	_T("PreTrack"),												_T("��ǰ����"),									//212
	_T("PostTrack"),											_T("���󵼹�"),									//213
	_T("Onload Track"),											_T("���ϵ���"),									//214
	_T("Debug:"),												_T("���Թ���"),									//215
	_T("216"),													_T("216"),										//216
	_T("Index"),												_T("�Ϻ�����"),									//217
	_T("Elevator"),												_T("����̨"),									//218
	_T("Arm Standby"),											_T("��е��Ԥ��λ"),								//219
	_T("Arm Sensor Distance:"),									_T("��е�ָ�Ӧ������:"),						//220
	_T("Magazine Thickness:"),									_T("�Ϻк��:"),								//221
	_T("Magazine Buffer Size:"),								_T("�Ϻл�����:"),								//222
	_T("First Empty Mag"),										_T("�׸������Ϻ�λ"),							//223
	_T("First Full Mag"),										_T("�׸������Ϻ�λ"),							//224
	_T("Empty Mag Position"),									_T("�����Ϻ�λ"),								//225
	_T("Full Mag Position"),									_T("�����Ϻ�λ"),								//226
	_T("Magazine Buffer"),										_T("�Ϻл���"),									//227
	_T("Magazine Storey:"),										_T("�Ϻв���:"),								//228
	_T("Empty Mag"),											_T("�����Ϻ�"),									//229
	_T("Full Mag"),												_T("�����Ϻ�"),									//230
	_T("Learn Pos"),											_T("��ѧλ��"),									//231
	_T("First Slot"),											_T("�Ϻ��ײ�"),									//232
	_T("Total Slots:"),											_T("�Ϻ��ܲ���:"),								//233
	_T("Slot Pitch:"),											_T("֧�ܲۼ��:"),								//234
	_T("Track Space:"),											_T("���⵼����:"),							//235
	_T("Position Adjust"),										_T("λ�õ���"),									//236
	_T("Adjust Item:"),											_T("������Ŀ:"),								//237
	_T("Magazine Position:"),									_T("�Ϻ�λ��:"),								//238
	_T("Adjust Step"),											_T("��������"),									//239
	_T("Move"),													_T("�ƶ�"),										//240
	_T("More"),													_T("��һЩ"),									//241
	_T("Less"),													_T("��һЩ"),									//242
	_T("Current Slot"),											_T("��ǰ֧�ܲ�"),								//243
	_T("Slot Verify"),											_T("֧�ܲ���֤"),								//244
	_T("Speed (mm/s)"),											_T("�ٶ� (mm/s)"),								//245
	_T("Clamp Stepper"),										_T("�о߲������"),								//246
	_T("Clamper:"),												_T("�о�:"),									//247
	_T("Index Stepper"),										_T("�����������"),								//248
	_T("Elevator Stepper"),										_T("����̨�������"),							//249
	_T("Index Speed:"),											_T("�Ϻ������ٶ�:"),							//250
	_T("Elevator Speed:"),										_T("����̨�ٶ�:"),								//251
	_T("Start"),												_T("��ʼ"),										//252
	_T("Stop"),													_T("ֹͣ"),										//253
	_T("Low"),													_T("��"),										//254
	_T("Middle"),												_T("��"),										//255
	_T("High"),													_T("��"),										//256
	_T("Clear"),												_T("���"),										//257
	_T("Up"),													_T("����"),										//258
	_T("Down"),													_T("����"),										//259
	_T("Arm Cylinder:"),										_T("��е������"),								//260
	_T("Rotary Cylinder:"),										_T("��ת����"),									//261
	_T("Both"),													_T("˫����"),									//262
	_T("Initialize"),											_T("��ʼ��"),									//263
	_T("Recover"),												_T("�ָ�"),										//264
	_T("Calibrate"),											_T("У׼"),										//265
	_T("Verify"),												_T("����"),										//266
	_T("Finish"),												_T("���"),										//267
	_T("Factors:"),												_T("ϵ��:"),									//268
	_T("Apply Force:"),											_T("ʩ����:"),									//269
	_T("Measured Force:"),										_T("������:"),									//270
	_T("Measure wire clamp force with force gauge "),			_T("�ò����Ʋ����߼мн���"),					//271
	_T("Key in measured force"),								_T("���������"),								//272
	_T("Step 1:"),												_T("��һ��:"),									//273
	_T("Step 2:"),												_T("�ڶ���:"),									//274
	_T("Calibration finished"),									_T("У׼���"),									//275
	_T("Verification finished"),								_T("�������"),									//276
	_T("Current:"),												_T("��ǰֵ"),									//277
	_T("New:"),													_T("����ֵ"),									//278
	_T("Operation"),											_T("����"),										//279
	_T("Fast"),													_T("��"),										//280
	_T("Very High"),											_T("�ܿ�"),										//281
	_T("Motion Type:"),											_T("�˶�����:"),								//282
	_T("Speed:"),												_T("�ٶ�:"),									//283
	_T("Empty Motion"),											_T("�����˶�"),									//284
	_T("Load Motion"),											_T("�����˶�"),									//285
	_T("Index Motion"),											_T("�Ϻ������˶�"),								//286
	_T("Search Motion"),										_T("�Ϻ������˶�"),								//287
	_T("Measure bond head force with force gauge"),				_T("�ò����Ʋ�����ͷѹ����"),					//288
	_T("Advanced Bond"),										_T("�߼�����"),									//289
	_T("Search Height (um)"),									_T("�����߶� (um)"),							//290
	_T("Search Threshold (um)"),								_T("������ֵ (um)"),							//291
	_T("Search Speed (mm/s)"),									_T("�����ٶ� (mm/s)"),							//292
	_T("Spark"),												_T("���Ӵ��"),									//293
	_T("Tail Length:"),											_T("��β����:"),								//294
	_T("Spark Delay"),											_T("�����ʱ"),									//295
	_T("Table position out of limit!"),							_T("����̨����������Χ!"),						//296
	_T("Table Jogging Speed:"),									_T("����̨�ƶ��ٶ�:"),							//297
	_T("Increase"),												_T("�ӿ�"),										//298
	_T("Decrease"),												_T("����"),										//299
	_T("LF Index"),												_T("����"),										//300	//V1.1.W149 edit
	_T("Unit Information"),										_T("��Ԫ��Ϣ"),									//301
	_T("Current Unit:"),										_T("��ǰ��Ԫ:"),								//302
	_T("Row"),													_T("��"),										//303
	_T("Column"),												_T("��"),										//304
	_T("Unit Layout"),											_T("��Ԫ����"),									//305
	_T("Bond Shift (um)"),										_T("��ͷ���� (um)"),							//306
	_T("Apply"),												_T("Ӧ��"),										//307
	_T("Wcl Tail Close (ms/2)"),								_T("�߼���β�߶ȹر� (ms/2)"),					//308
	_T("Bond Timeout!"),										_T("���ӳ�ʱ!"),								//309
	_T("Servo Error!"),											_T("�ŷ�ϵͳ����!"),							//310
	_T("Spark Failure!"),										_T("���Ӵ�����!"),							//311
	_T("1st Bond Nonstick!"),									_T("һ���㲻ճ!"),								//312
	_T("2nd Bond Nonstick!"),									_T("�����㲻ճ!"),								//313
	_T("Power Delay (ms)"),										_T("������ʱ (ms)"),							//314
	_T("Tail Shift (um)"),										_T("��β���� (um)"),							//315
	_T("Offload Delay:"),										_T("ж����ʱ:"),								//316
	_T("Kicker:"),												_T("��Ƭ:"),									//317
	_T("Holder:"),												_T("֧��:"),									//318
	_T("Pusher"),												_T("�ƶ���"),									//319
	_T("Cylinder"),												_T("����"),										//320
	_T("Bond Calculate Error!"),								_T("�������!"),								//321
	_T("Pusher Stepper"),										_T("�ƶ����������"),							//322
	_T("Start Position:"),										_T("��ʼλ��:"),								//323
	_T("Push"),													_T("�ƶ�"),										//324
	_T("Back"),													_T("����"),										//325
	_T("Onload Stepper"),										_T("���ϲ������"),								//326
	_T("LF Thickness:"),										_T("֧�ܺ��:"),								//327
	_T("Push Distance:"),										_T("�ƶ�����:"),								//328
	_T("Calibration factor out of range"),						_T("У׼ϵ��������Ч��Χ!"),					//329
	_T("Please calibrate again"),								_T("������У׼!"),								//330
	_T("Neck Height (um)"),										_T("�߾��߶� (um)"),							//331
	_T("Operator"),												_T("������"),									//332
	_T("Process"),												_T("����Ա"),									//333
	_T("Service"),												_T("ά��Ա"),									//334
	_T("Wcl Loop Peak Close (ms/2)"),							_T("�߼л����ر� (ms/2)"),						//335
	_T("Heater temperature out of range!"),						_T("�������¶ȳ�����Ч��Χ!"),					//336
	_T("Claw"),													_T("ץƬ"),										//337
	_T("Limit"),												_T("�ƶ�����"),									//338
	_T("Trajectory Factor (%)"),								_T("�߻��켣ϵ�� (%)"),							//339
	_T("BSD Network Error"),									_T("BSD�������"),								//340
	_T("BSD Socket Error"),										_T("BSDЭ�����"),								//341
	_T("BSD Communication Error"),								_T("BSDͨѶ����"),								//342
	_T("BSD Version Error"),									_T("BSD�汾����"),								//343
	_T("EFO Init Error"),										_T("EFO��ʼ������"),							//344
	_T("Stick Detection Configure"),							_T("���㲻ճ�������"),							//345
	_T("Hardware Parameters"),									_T("Ӳ������"),									//346
	_T("Apply Voltage (V):"),									_T("ʩ�ӵ�ѹ (V):"),							//347
	_T("Die Detect Threshold (V):"),							_T("оƬ�����ֵ (V):"),						//348
	_T("Lead Detect Threshold (V):"),							_T("�ܽż����ֵ (V):"),						//349
	_T("Tail Short Threshold:"),								_T("��β̫����ֵ:"),							//350
	_T("Nonstick Threshold:"),									_T("���㲻ճ��ֵ:"),							//351
	_T("Bond Detect Parameters"),								_T("���������"),								//352
	_T("Tail Too Short!"),										_T("��β̫��!"),								//353
	_T("Pre Clamp Position:"),									_T("Ԥ�ӽ�λ��:"),								//354
	_T("Lead Frame Index Time Out!"),							_T("֧�����ϳ�ʱ!"),							//355
	_T("Update"),												_T("����"),										//356
	_T("Thread Wire Position"),									_T("����λ��"),									//357
	_T("Clear Track"),											_T("��չ��"),									//358
	_T("Skip"),													_T("����"),										//359
	_T("Onload Push Kicker Wrong!"),							_T("������Ƭ������!"),							//360
	_T("Onload Picker Jam!"),									_T("���ϻ�е�ֿ�ס!"),							//361
	_T("Onload Pully Jam!"),									_T("�����Ϻ��ƶ�����ס!"),						//362
	_T("Claw Missing Steps, Please Home it!"),					_T("��צ�����������¸�λ!"),					//363
	_T("Offload Kicker Wrong!"),								_T("������Ƭ����!"),							//364
	_T("Offload Kicker Jam!"),									_T("������Ƭ��ס!"),							//365
	_T("Offload Claw Wrong!"),									_T("����ץƬ����!"),							//366
	_T("Offload Claw Jam!"),									_T("����ץƬ��ס!"),							//367
	_T("Offload Inner Pusher Wrong!"),							_T("�����ڲ��ƶ�������!"),						//368
	_T("Offload Pusher Jam!"),									_T("�����ƶ�����ס!"),							//369
	_T("Offload Outer Pusher Wrong!"),							_T("��������ƶ�������!"),						//370
	_T("Offload Outer Pusher Jam!"),							_T("��������ƶ�����ס!"),						//371
	_T("Offload Inner Holder Wrong!"),							_T("�����ڲ�֧�ų���!"),						//372
	_T("Offload Inner Holder Jam!"),							_T("�����ڲ�֧�ſ�ס!"),						//373
	_T("Offload Outer Holder Wrong!"),							_T("�������֧�ų���!"),						//374
	_T("Offload Outer Holder Jam!"),							_T("�������֧�ſ�ס!"),						//375
	_T("Control"),												_T("���Ʋ���"),									//376
	_T("Pre Impact Force (gm)"),								_T("ǰ��Ԥ��ѹ�� (gm)"),						//377	//V1.1.W149 edit
	_T("Advanced Loop"),										_T("�߼��߻�"),									//378
	_T("Search Delay (ms)"),									_T("������ʱ (ms)"),							//379
	_T("Pre Heat Time:"),										_T("Ԥ��ʱ��:"),								//380
	_T("PR Pass % (Die/Lead)"),									_T("PR������ % (оƬ/�ܽ�)"),					//381
	_T("Reset"),												_T("�����¼"),									//382
	_T("Machine ID"),											_T("����ʶ���"),								//383	//V1.1.W258 edit
	_T("Start Time:"),											_T("��ʼʱ��:"),								//384
	_T("Output:"),												_T("����:"),									//385
	_T("Production:"),											_T("����ʱ��:"),								//386
	_T("Standby:"),												_T("����ʱ��:"),								//387
	_T("Assist"),												_T("��������"),									//388	//v1.5.T71 edit
	_T("Failure"),												_T("ʧ�ܴ���"),									//389	//v1.5.T71 edit
	_T("Wire:"),												_T("������:"),									//390
	_T("Tool Usage:"),											_T("�ɾ�����:"),								//391
	_T("EFO Open:"),											_T("���·:"),								//392
	_T("Short Tail:"),											_T("��β����:"),								//393
	_T("Assistance"),											_T("����ͳ��"),									//394
	_T("Threshold"),											_T("��ֵ"),										//395
	_T("Reference Information"),								_T("����ϵ��Ϣ"),								//396
	_T("Contact Parameters"),									_T("�Ӵ�����������"),							//397
	_T("Vision Factors"),										_T("ͼ��ֱ���ϵ��"),							//398
	_T("Setting"),												_T("����"),										//399
	_T("Magnification"),										_T("�Ŵ���:"),								//400
	_T("Pattern Size X"),										_T("ģ��ߴ�X:"),								//401
	_T("Pattern Size Y"),										_T("ģ��ߴ�Y:"),								//402
	_T("Time (ms)"),											_T("ʱ�� (ms)"),								//403
	_T("Power (Dac)"),											_T("���� (Dac)"),								//404
	_T("Continue"),												_T("����"),										//405
	_T("Teach Alignment Point"),								_T("ָ����λ��"),								//406
	_T("Reference Number"),										_T("����ϵ����"),								//407
	_T("Lens Setting"),											_T("��ͷ����"),									//408
	_T("Die Number"),											_T("оƬ����"),									//409
	_T("Pattern Parameters"),									_T("ģ�����"),									//410
	_T("Vision Algorithm"),										_T("ͼ��ʶ���㷨"),								//411
	_T("Locate Mode"),											_T("��λģʽ"),									//412
	_T("Grey Level"),											_T("�Ҷ�ͼ��"),									//413
	_T("Binary"),												_T("�ڰ�ͼ��"),									//414
	_T("Dual Point"),											_T("���㶨λ"),									//415
	_T("Single Point"),											_T("���㶨λ"),									//416
	_T("Standard"),												_T("��׼"),										//417
	_T("Flat"),													_T("ƽ��"),										//418
	_T("Mode A"),												_T("ģʽ A"),									//419
	_T("Mode B"),												_T("ģʽ B"),									//420
	_T("Bond Mode"),											_T("����ģʽ"),									//421
	_T("Bond Control"),											_T("���߿���"),									//422
	_T("Motor"),												_T("���"),										//423
	_T("System"),												_T("ϵͳ"),										//424
	_T("New"),													_T("�½�"),										//425
	_T("Edit"),													_T("�༭"),										//426
	_T("Delete"),												_T("ɾ��"),										//427
	_T("Insert"),												_T("����"),										//428
	_T("Teach Wire Layout"),									_T("ָ�����߲���"),								//429
	_T("Wire Property"),										_T("��������"),									//430
	_T("1st Bond On"),											_T("һ����λ��"),								//431
	_T("2nd Bond On"),											_T("������λ��"),								//432
	_T("Wire Layout"),											_T("���߲���"),									//433
	_T("Wire Num"),												_T("�ߺ�"),										//434
	_T("Sequence Setting"),										_T("����˳������"),								//435
	_T("Program Type"),											_T("��ʽ����"),									//436
	_T("No. of Row/Col"),										_T("����/����"),								//437	//v1.5.T91 edit
	_T(""),														_T(""),											//438	//v1.5.T91 edit
	_T("PR Ahead Col"),											_T("Ԥ��λ����"),								//439
	_T("Bond Sequence"),										_T("����˳��"),									//440	//v1.5.T91 edit
	_T("Track Type"),											_T("��������"),									//441
	_T("Unit Layout"),											_T("��Ԫ����"),									//442
	_T("None"),													_T("��"),										//443
	_T("Matrix"),												_T("����"),										//444
	_T("Hybrid"),												_T("�ϳ�"),										//445
	_T("Die Ahead"),											_T("оƬԤ��λ"),								//446
	_T("Die Lead Ahead"),										_T("оƬ�ܽ�Ԥ��λ"),							//447
	_T("General Profile"),										_T("ͨ������"),									//448
	_T("Thread Wire Open Force"),								_T("�����ſ���"),								//449
	_T("Thread Wire Close Force"),								_T("���߼н���"),								//450
	_T("Hold Wire Close Force"),								_T("���üн���"),								//451
	_T("Bond Profile"),											_T("��������"),									//452
	_T("Profile Type"),											_T("��������"),									//453
	_T("1st Step Force"),										_T("��һ������"),								//454
	_T("1st Step Ramp Time"),									_T("��һ��ת��ʱ��"),							//455
	_T("1st Step Hold Time"),									_T("��һ������ʱ��"),							//456
	_T("2nd Step Force"),										_T("�ڶ�������"),								//457
	_T("2nd Step Ramp Time"),									_T("�ڶ���ת��ʱ��"),							//458
	_T("2nd Step Hold Time"),									_T("�ڶ�������ʱ��"),							//459
	_T("Clamp Force"),											_T("�н���"),									//460
	_T("Clamp Ramp Time"),										_T("�н���ת��ʱ��"),							//461
	_T("Ball Sit Open/Close"),									_T("��λ�õ����ſ�/�н�"),						//462
	_T("Search Height Open"),									_T("�����߶��ſ�"),								//463
	_T("Peak Height Close"),									_T("�����߶ȼн�"),								//464
	_T("Tail Height Close"),									_T("��β�߶ȼн�"),								//465
	_T("Stop All"),												_T("��ͣ"),										//466
	_T("Parameter out of range!"),								_T("����������Χ!"),							//467
	_T("Software Version"),										_T("����汾"),									//468
	_T("Wcl 1st Bond Open (ms/2)"),								_T("�߼�һ���ſ� (ms/2)"),						//469
	_T("Wcl 2nd Bond Open (ms/2)"),								_T("�߼ж����ſ� (ms/2)"),						//470
	_T("WCL Loop Peak Close Time (ms)"),						_T("�߼л����н�ʱ�� (ms)"),					//471
	_T("Onload Fork Not Exist!"),								_T("�����ϲ治��!"),							//472
	_T("Offload Fork Not Exist!"),								_T("�����ϲ治��!"),							//473
	_T("Offload Outer Fork Not Exist!"),						_T("��������ϲ治��!"),						//474
	_T("Offload Fork Full!"),									_T("�����ϲ�����!"),							//475
	_T("Offload Outer Fork Full!"),								_T("��������ϲ�����!"),						//476
	_T("Gap exist between Lf to Lf!"),							_T("֧��֮���м�϶��"),							//477
	_T("Image Transfer Time (ms):"),							_T("ͼ����ʱ��(ms):"),						//478
	_T("Disable"),												_T("����"),										//479
	_T("Enable"),												_T("ʹ��"),										//480
	_T("Track Flatness"),										_T("����ˮƽ��"),								//481
	_T("Press Start to check track flatness"),					_T("��'��ʼ'����鵼��ˮƽ��"),					//482
	_T("Move table to inner track left position"),				_T("�ƶ�����̨���ڲർ�����λ��"),				//483
	_T("Move table to inner track middle position"),			_T("�ƶ�����̨���ڲർ���м�λ��"),				//484
	_T("Move table to inner track right position"),				_T("�ƶ�����̨���ڲർ���Ҳ�λ��"),				//485
	_T("Move table to outer track left position"),				_T("�ƶ�����̨����ർ�����λ��"),				//486
	_T("Move table to outer track middle position"),			_T("�ƶ�����̨����ർ���м�λ��"),				//487
	_T("Move table to outer track right position"),				_T("�ƶ�����̨����ർ���Ҳ�λ��"),				//488
	_T("Track level list:"),									_T("����߶��б�:"),							//489
	_T("BSD"),													_T("��ճ���"),									//490
	_T("Scrub"),												_T("��ĥ"),										//491
	_T("Scrub Group"),											_T("��ĥ���"),									//492
	_T("Scrub Offset1 (um)"),									_T("��ĥƫ��1 (um)"),							//493
	_T("Scrub Offset2 (um)"),									_T("��ĥƫ��2 (um)"),							//494
	_T("Cycle"),												_T("ѭ������"),									//495
	_T("Amplitude (um)"),										_T("���� (um)"),								//496
	_T("Direction"),											_T("����"),										//497
	_T("Speed (%)"),											_T("�ٶ� (%)"),									//498
	_T("Vertical"),												_T("����"),										//499
	_T("Auto Skip Die/Unit"),									_T("�Զ�����оƬ/��Ԫ"),						//500
	_T("Search Range"),											_T("������Χ"),									//501
	_T("Bond Process Configure"),								_T("���ӹ�������"),								//502
	_T("Safety"),												_T("��ȫ����"),									//503
	_T("Capillary Wear Out Limit:"),							_T("�ɾ�ĥ����:"),							//504
	_T("Capillary Usage"),										_T("�ɾ�ʹ�����"),								//505
	_T("Limit:"),												_T("����:"),									//506
	_T("Count"),												_T("����"),										//507	//v1.5.T71 edit
	_T("Capillary exceed Wear Out Limit"),						_T("�ɾ�ĥ�𳬳�����"),							//508
	_T("Please change capillary"),								_T("������ɾ�"),								//509
	_T("Red"),													_T("���"),										//510
	_T("Side Light"),											_T("���"),										//511	//V1.1.W133 edit
	_T("Blue"),													_T("����"),										//512
	_T("Force"),												_T("ѹ��"),										//513
	_T("Confirm"),												_T("ȷ��"),										//514
	_T("Tail Length"),											_T("��β����"),									//515
	_T("Current Fire Level"),									_T("��ǰ����߶�"),								//516
	_T("New Fire Level"),										_T("������߶�"),								//517
	_T("This wire is bonded"),									_T("��ǰ���Ѿ�����,"),							//518
	_T("Please confirm to continue or not"),					_T("��ȷ���Ƿ����"),							//519
	_T("Trajectory Mode"),										_T("�߻��켣ģʽ"),								//520
	_T("All"),													_T("ȫ��"),										//521
	_T("Odd"),													_T("����"),										//522
	_T("Even"),													_T("ż��"),										//523
	_T("AUX PR"),												_T("ģ�屸��"),									//524
	_T("Alignment"),											_T("��λ��"),									//525
	_T("Bond Position"),										_T("����"),										//526
	_T("Onload Fork Empty!"),									_T("���Ϻп�!"),								//527
	_T("Please Onload Lead Frame!"),							_T("����װ��֧��!"),							//528
	_T("Bond Time 1"),											_T("����ʱ��һ"),								//529
	_T("Bond Power 1"),											_T("��������һ"),								//530
	_T("Bond Force 1"),											_T("����ѹ��һ"),								//531
	_T("Bond Time 2"),											_T("����ʱ���"),								//532
	_T("Bond Power 2"),											_T("����������"),								//533
	_T("Bond Force 2"),											_T("����ѹ����"),								//534
	_T("Fork Capacity"),										_T("���Ϻ�����:"),								//535
	_T("Inner Track Offload Fork Full!"),						_T("�ڲ������Ϻ�����!"),						//536
	_T("Please Offload Lead Frame!"),							_T("��ж��֧��!"),								//537
	_T("Outer Track Offload Fork Full!"),						_T("��������Ϻ�����!"),						//538
	_T("Shift Speed (%)"),										_T("�����ٶ�(%)"),								//539
	_T("Coax Light"),											_T("ͬ���"),									//540	//V1.1.W133 edit
	_T("Die Tol Fail"),											_T("оƬ��λ��೬��"),							//541
	_T("Lead Tol Fail"),										_T("�ܽŶ�λ��೬��"),							//542
	_T("Alignment Tolerance"),									_T("��λ�����ֵ"),								//543
	_T("Redo PR"),												_T("�ٶ�λ"),									//544
	_T("(0) Disable (1) Enable"),								_T("(0) ����� (1) ���"),						//545
	_T("(0) Wire (1) BBOS (2) BSOB (3) BSB"),					_T("(0) ������ (1) ѹ�� (2) ���� (3) ѹ����"),	//546	//v3.1.T90 edit
	_T("Bump Ball"),											_T("����"),										//547
	_T("BSOB Parameter"),										_T("�������"),									//548
	_T("Shape"),												_T("����"),										//549
	_T("Bond Ball Mode"),										_T("������ʽ"),									//550
	_T("BBOS"),													_T("ѹ��"),										//551	//v3.0.T429 rename
	_T("BSOB"),													_T("����"),										//552	//v3.0.T429 rename
	_T("2nd Bond Offset (um)"),									_T("����ƫ�� (um)"),							//553
	_T("BSOB Base Height (um)"),								_T("��������߶� (um)"),						//554
	_T("BSOB Motion Speed (%)"),								_T("�����˶��ٶ� (%)"),							//555
	_T("BSOB Scrub Distance (um)"),								_T("������о��� (um)"),						//556
	_T("BSOB Span Length (um)"),								_T("����б�糤�� (um)"),						//557
	_T("BSOB Ball Thickness (um)"),								_T("�������κ�� (um)"),						//558
	_T("Bond Sequence Error!"),									_T("���ߴ������!"),							//559
	_T("Wire and Ball"),										_T("�ߺ���"),									//560
	_T("Ball Only"),											_T("��"),										//561
	_T("Wire Only"),											_T("��"),										//562
	_T("Bond Object"),											_T("���Ӷ���"),									//563
	_T("Bump Wire"),											_T("���߲���"),									//564
	_T("Bump Ball"),											_T("�������"),									//565
	_T("No Alignment"),											_T("û�ж�λ"),									//566
	_T("Press 'Start' to teach contact level"),					_T("��'��ʼ'ָ���Ӵ���߶�"),					//567
	_T("Teach contact level of "),								_T("��Ѱ�Ӵ��߶�: "),							//568
	_T("Loop Top Mode"),										_T("����ģʽ"),									//569
	_T("Please index leadframe manually"),						_T("���ֶ�����֧��"),							//570
	_T("USG"),													_T("����"),										//571
	_T("USG Ramp Time"),										_T("����ת��ʱ��"),								//572
	_T("Thermal Expansion"),									_T("������"),									//573
	_T("Expansion Handle:"),									_T("���ʹ���:"),								//574
	_T("Handle Time:"),											_T("����ʱ��:"),								//575
	_T("Pre Heater"),											_T("Ԥ������"),									//576
	_T("Sensor Switch Time"),									_T("��Ӧ��ת��ʱ��"),							//577
	_T("Onload Inner Track no leadframe"),						_T("�����ڲ�����֧��"),						//578
	_T("Onload Outer Track no leadframe"),						_T("�����������֧��"),						//579
	_T("Track leadframe not exist"),							_T("���֧�ܲ�����"),							//580
	_T("Reset"),												_T("��λ"),										//581
	_T("Onload Track Standby"),									_T("���Ϲ��Ԥ��ʱ��"),							//582
	_T("Bond Track Standby"),									_T("���ӹ��Ԥ��ʱ��"),							//583
	_T("Onload track jam"),										_T("���Ϲ��֧�ܿ�ס"),							//584
	_T("Onload outer track jam"),								_T("���������֧�ܿ�ס"),						//585
	_T("Impact Force 2"),										_T("���ѹ����"),								//586
	_T("Feed Power"),											_T("��������"),									//587
	_T("Current:"),												_T("��ǰ��:"),									//588
	_T("Restart:"),												_T("��ʼ��:"),									//589
	_T("Total Wire:"),											_T("������:"),									//590
	_T("NSOL Sample:"),											_T("����������:"),							//591
	_T("Heater"),												_T("������"),									//592
	_T("Bond Para"),											_T("���Ӳ���"),									//593
	_T("Loop Para"),											_T("�߻�����"),									//594
	_T("Wire Usage"),											_T("������"),									//595
	_T("Wires:"),												_T("������:"),									//596
	_T("Software Manager"),										_T("�������"),									//597
	_T("Upgrade"),												_T("�������"),									//598
	_T("Current Version"),										_T("��ǰ�汾"),									//599
	_T("Date Time"),											_T("����ʱ��"),									//600
	_T("Software Upgrade"),										_T("�������"),									//601
	_T("Restart Setting"),										_T("����������"),								//602
	_T("Bonder"),												_T("����ϵͳ"),									//603
	_T("MHS"),													_T("����ϵͳ"),									//604
	_T("Servo"),												_T("�ŷ�ϵͳ"),									//605
	_T("Cold Start Items:"),									_T("������ѡ��:"),								//606
	_T("Press Edit to edit wire bond point"),					_T("��'�༭'���༭����λ��"),					//607
	_T("Press New to teach new wire bond point"),				_T("��'����'��ѧϰ����λ��"),					//608
	_T("Press Start to edit program"),							_T("��'��ʼ'���༭��ʽ"),						//609
	_T("Press Prev/Next to change wire no."),					_T("��'ǰһ��','��һ��'�ı��ߺ�"),				//610
	_T("Bond Head"),											_T("��ͷ"),										//611
	_T("Safe Position"),										_T("��ȫλ��"),									//612
	_T("Load Control Parameter"),								_T("���ؿ��Ʋ���"),								//613
	_T("Position"),												_T("λ��"),										//614
	_T("Load"),													_T("����"),										//615
	_T("Engineer"),												_T("����ʦ"),									//616
	_T("PR Pattern"),											_T("PRģ��"),									//617
	_T("Wire"),													_T("����"),										//618
	_T("Range"),												_T("������Χ"),									//619
	_T("Lower"),												_T("����"),										//620
	_T("Upper"),												_T("����"),										//621
	_T("Vision"),												_T("�Ӿ�����"),									//622
	_T("Light"),												_T("�ƹ����"),									//623
	_T("Wire roll gold wire use up, please check��"),			_T("����������꣬���飡"),					//624
	_T("Are you sure to delete current bond program record?"),	_T("ȷ��ɾ����ǰ��ʽ��"),						//625
	_T("Coax Blue"),											_T("��ɫͬ���"),								//626
	_T("Coax Red"),												_T("��ɫͬ���"),								//627
	_T("Side Blue"),											_T("��ɫ���"),									//628
	_T("Side Red"),												_T("��ɫ���"),									//629
	_T("Threshold"),											_T("��ֵ"),										//630
	_T(">>"),													_T(">>"),										//631
	_T("<<"),													_T("<<"),										//632  
	_T("Material Handler"),										_T("����ϵͳ"),									//633
	_T("PreHeat"),												_T("�н�����"),									//634
	_T("BSD Debug"),											_T("BSD ������"),								//635
	_T("BSD Debug Info Record"),								_T("BSD �����Ϣ����"),							//636
	_T("Recommended BSD Parameters"),							_T("BSD �Ƽ���������"),							//637
	_T("Please Confirm no Error on previous 5 units!"),			_T("��ǰ5���������밴ȷ�ϣ�"),				//638
	_T("Apply"),												_T("�������"),									//639
	_T("Clear All"),											_T("���ȫ��"),									//640
	_T("AutoSample On"),										_T("�����Զ�����"),								//641
	_T("Fake False Alarm Detected,press ok to auto sampling"),	_T("���ܳ����󱨣�ȷ�Ͽ����Զ�������"),			//642
	_T("Die Detect:(V):"),										_T("оƬ�����ֵ (V):"),						//643
	_T("Lead Detect: (V):"),									_T("�ܽż����ֵ (V):"),						//644
	_T("Tail Short:"),											_T("��β̫����ֵ:"),							//645
	_T("Nonstick:"),											_T("���㲻ճ��ֵ:"),							//646
	_T("1st Power:"),											_T("1������:"),									//647
	_T("2nd Power:"),											_T("2������:"),									//648
	_T("1st Force :"),											_T("1��ѹ��:"),									//649
	_T("2nd Force:"),											_T("2��ѹ��:"),									//650
	_T("Program:"),												_T("��ʽ:"),									//651
	_T("1st Bond Position:"),									_T("Ԥ��һ������:"),							//652
	_T("2nd Bond Position:"),									_T("Ԥ���������:"),							//653
	_T("Recipe:"),												_T("��ʽ��"),									//654
	_T("No Program"),											_T("��ʽδ����"),								//655
	_T("Loop"),													_T("�߻�"),										//656
	_T("Neck Height Pre-Move (%)"),								_T("�߾��߶���ǰ�ƶ� (%)"),						//657
	_T("Neck Distance Pre-Move (%)"),							_T("�߾�������ǰ�ƶ� (%)"),						//658
	_T("Kink2 Height Pre-Move (%)"),							_T("�յ�2�߶���ǰ�ƶ� (%)"),					//659	//v3.1.T320 edit
	_T("Kink2 Distance Pre-Move (%)"),							_T("�յ�2������ǰ�ƶ� (%)"),					//660	//v3.1.T320 edit
	_T("Loop Top Pre-Move (%)"),								_T("������ǰ�ƶ� (%)"),							//661
	_T("PR Version"),											_T("PR �汾"),									//662
	_T("No Elec Power.Auto shut down in 5 sec"),				_T("���е磬��������5�����˳���"),				//663 
	_T("Last LF"),												_T("���֧��"),									//664
	_T("1st Bond Boost"),										_T("һ������"),									//665
	_T("Duration (Impact/Base)"),								_T("����ʱ�� (���/����)"),						//666
	_T("Mode (Impact/Base)"),									_T("����ģʽ (���/����)"),						//667
	_T("Amplitude (Impact/Base)"),								_T("���� (���/����)"),							//668
	_T("Short"),												_T("��"),										//669
	_T("Long"),													_T("��"),										//670
	_T("Frequency"),											_T("г��Ƶ��"),									//671
	_T("Impedance"),											_T("г���迹"),									//672
	_T("Transducer"),											_T("����У׼"),									//673
	_T("Seat (Dac/ms)"),										_T("����(Dac/ms)"),								//674
	_T("Feed (Dac/ms)"),										_T("����(Dac/ms)"),								//675
	_T("Tail (Dac/ms)"),										_T("��β(Dac/ms)"),								//676 
	_T("Seat (ms):"),											_T("����(ms):"),								//677
	_T("Feed (ms):"),											_T("����(ms):"),								//678
	_T("Tail (ms):"),											_T("��β(ms):"),								//679
	_T("USG Error occur��Please check calibration first!:"),		_T("��������������ȼ��У׼״̬��"),		//680
	_T("Time:"),												_T("ʱ��:"),									//681
	_T("Level:"),												_T("��С��"),									//682
	_T("Generate"),												_T("���"),										//683
	_T("Voltage"),												_T("��ѹ"),										//684
	_T("Current"),												_T("����"),										//685
	_T("Power"),												_T("�㹦��"),									//686
	_T("Mode"),													_T("ģʽ"),										//687
	_T("USG Setup"),											_T("��������趨"),								//688
	_T("High Frequency"),										_T("��ƵУ׼"),									//689
	_T("Low Frequency"),										_T("��ƵУ׼"),									//690
	_T("USG Output Generate "),									_T("�������"),									//691
	_T("USG Calibrate Success!"),								_T("����У׼�ɹ���"),							//692
	_T("Impedance Out of Range"),								_T("�迹������Χ��"),							//693
	_T("USG Calibrate Failed!"),								_T("����У׼ʧ�ܣ�"),							//694	//v3.1.T1 edit
	_T("File"),													_T("�ļ�����"),									//695
	_T("Diagnostic Tool"),										_T("��Ϲ���"),									//696
	_T("Copper Sequence"),										_T("ͭ��˳��"),									//697
	_T("Lead Frame Information"),								_T("֧����Ϣ"),									//698
	_T("Total Units"),											_T("֧�ܵ�Ԫ��"),								//699
	_T("Index Times"),											_T("���ϴ���"),									//700
	_T("Onload Picker"),										_T("����ץ��"),									//701 
	_T("Injector Stepper"),										_T("������"),									//702
	_T("Offload Kicker"),										_T("��������"),									//703
	_T("Material Handling System Utility"),						_T("ԭ�ϴ���ϵͳ����"),							//704
	_T("Left Claw"),											_T("��ದצ"),									//705
	_T("Right Claw"),											_T("�Ҳದצ"),									//706
	_T("Compensation"),											_T("λ�ò���"),									//707
	_T("Clamper"),												_T("�о߿���"),									//708
	_T("SingleIndex"),											_T("��������"),									//709
	_T("Lead PR:"),												_T("�ܽ�ʶ��:"),								//710
	_T("Contact Level Search"),									_T("�Ӵ���Ӵ�����"),							//711
	_T("Press Start to Begin a New Test"),						_T("����ʼ������һ�����"),						//712
	_T("Onload"),												_T("����"),										//713
	_T("Track Front"),											_T("����ǰ��"),									//714
	_T("Track Back"),											_T("�������"),									//715
	_T("Sensor & Cylinder "),									_T("��������������"),							//716
	_T("Stepper"),												_T("�������"),									//717
	_T("Onload Track Double Lead Frame!"),						_T("���Ϲ��˫֧��!"),							//718
	_T("LF standby Unit:"),										_T("��ʼ֧�ܵ�Ԫ:"),							//719
	_T("Left Claw to Bond Area Index Times:"),					_T("��צ�����ӵ��������:"),					//720
	_T("Bond Area to Right Claw Index Times:"),					_T("���ӵ����Ҳ�צ�������:"),					//721
	_T("Preheat Delay(ms):"),									_T("Ԥ������ʱ(ms):"),							//722
	_T("Clamp Open Pre-Trigger Claw (%):"),						_T("�оߴ�Ԥ������צ(%):"),					//723
	_T("Claw Pre-Trigger Clamp Close (%):"),					_T("��צԤ�����о߹ر�(%):"),					//724
	_T("Claw Catch Up:"),										_T("׷���ٶ�:"),								//725
	_T("Index:"),												_T("֧������"),									//726
	_T("Index Speed"),											_T("�����ٶ�"),									//727
	_T("Double Check Position:"),								_T("˫֧���λ��:"),							//728
	_T("Inject Position:"),										_T("֧���ƽ�λ��:"),							//729
	_T("CheckPos"),												_T("���λ"),									//730
	_T("Inject"),												_T("�ƽ�λ"),									//731
	_T("Run With Sensors"),										_T("���������"),								//732
	_T("Stepper Time Test"),									_T("����ʱ�����"),								//733
	_T("Sensor"),												_T("������"),									//734
	_T("Index Cycle Time (ms):"),								_T("��������ʱ��(ms):"),						//735
	_T("Exposure Time (us):"),									_T("�ع�ʱ��(us):"),							//736
	_T("Feature"),												_T("����"),										//737
	_T("Constant Wire Length:"),								_T("�߳��̶�:"),								//738
	_T("Table"),												_T("����̨"),									//739
	_T("X Limit"),												_T("X��λ"),									//740
	_T("Y Limit"),												_T("Y��λ"),									//741
	_T("Will Start to Search Table Limit,"),					_T("������������̨��λ,"),						//742
	_T("Please Make Sure Bond Area Clear!"),					_T("��ȷ�Ϻ����������赲!"),					//743
	_T("Search Table Limit Finish!"),							_T("��������̨��λ���!"),						//744
	_T("Position compensation in progress..."),					_T("λ�ò���������..."),						//745
	_T("Position compensation success!"),						_T("λ�ò����ɹ�!"),							//746
	_T("Position compensation out of range!"),					_T("λ�ò���������Χ!"),						//747
	_T("Position compensation fail!"),							_T("λ�ò���ʧ��!"),							//748
	_T("Press 'Bond' to make capillary mark "),					_T("��'����'ѹ���ɾ�ӡ"),						//749	//v3.0.T454 edit
	_T("Press 'Verify' to check capillary mark"),				_T("��'����'���ɾ�ӡ"),						//750
	_T("Bond"),													_T("����"),										//751
	_T("OK"),													_T("����"),										//752 
	_T("Press 'OK' to continue"),								_T("��'OK'����"),								//753 
	_T("PR Retry"),												_T("PR����"),									//754
	_T("Please Update Contact Level!"),							_T("����½Ӵ���߶�!"),						//755
	_T("Light"),												_T("�ƹ�"),										//756 
	_T("Index"),												_T("����"),										//757 
	_T("Adjust"),												_T("΢��"),										//758 
	_T("Thread"),												_T("����"),										//759
	_T("Oscillate"),											_T("��"),										//760 
	_T("Zero BTOS"),											_T("��ɾ�ƫ��"),								//761
	_T("AFP Mode"),												_T("װ������"),									//762
	_T("IO"),													_T("�������"),									//763
	_T("Encoder"),												_T("������"),									//764
	_T("Auto Tune"),											_T("�Զ�����"),									//765
	_T("Fork Switch Buffer"),									_T("������������:"),							//766
	_T("Remove Window Clamper"),								_T("ȡ�¼о�"),									//767
	_T("PR Fail!"),												_T("ͼ���޷�ʶ��"),							//768
	_T("Material Handler Calibration"),							_T("����ϵͳУ׼"),								//769
	_T("PR Index"),												_T("����ͼ��ѧϰ"),								//770
	_T("Device"),												_T("�豸����"),									//771
	_T("Setting"),												_T("���ò���"),									//772
	_T("Bond Unit"),											_T("���ӵ�Ԫ"),									//773
	_T("Sensor to Bond Area Units:"),							_T("���֧��ƫ������"),							//774
	_T("FAB Test"),												_T("FAB ����"),									//775
	_T("Thermal Protection Time:"),								_T("���ȱ���ʱ��:"),							//776
	_T("Correct Bond Offset"),									_T("���߸��Ӻ��Ӳ���"),							//777
	_T("Power Compensation"),									_T("��������"),									//778
	_T("Contact Level Offset"),									_T("�Ӵ���ƫ����"),								//779
	_T("Circle Size"),											_T("�����С"),									//780
	_T("Bump Ball Control"),									_T("�������"),									//781
	_T("File Name Too Long!"),									_T("�ļ�������!"),								//782
	_T("Indexer"),												_T("ץƬ��"),									//783
	_T("Test"),													_T("����"),										//784
	_T("Database"),												_T("����ѡ��"),									//785
	_T("Magazine"),												_T("�Ϻ�"),										//786
	_T("Leadframe"),											_T("֧��"),										//787
	_T("Pitch:"),												_T("�м��:"),									//788
	_T("Bottom to 1st Slot:"),									_T("���ظ�:"),									//789
	_T("Last Slot to Top:"),									_T("���ظ�:"),									//790
	_T("Edge length:"),											_T("���ؿ�:"),									//791
	_T("Clamp Column"),											_T("�о�����"),									//792
	_T("1st Heat Delay (ms)"),									_T("��ʼԤ������ʱ(ms)"),						//793
	_T("Heat Delay (ms)"),										_T("Ԥ������ʱ(ms)"),							//794
	_T("PR Index"),												_T("����ͼ��"),									//795
	_T("PR Index Tol (um)"),									_T("����ͼ����ֵ(um)"),							//796
	_T("Srch Back Length"),										_T("������������"),								//797
	_T("Standby Length (mm)"),									_T("���ϵȴ�λ��(mm)"),							//798
	_T("Eject Speed"),											_T("֧���Ƴ��ٶ�"),								//799
	_T("Right Clearance (mm)"),									_T("�Ҽ�����ֵ(mm)"),							//800
	_T("Slowest"),												_T("����"),										//801
	_T("Slower"),												_T("����"),										//802
	_T("Slow"),													_T("��"),										//803
	_T("Medium"),												_T("�е�"),										//804
	_T("Fast"),													_T("��"),										//805
	_T("Faster"),												_T("�Ͽ�"),										//806
	_T("Fastest"),												_T("���"),										//807
	_T("Preclamp Position:"),									_T("Ԥ�н�λ�ã�"),								//808
	_T("Preclamp"),												_T("Ԥ�н�"),									//809
	_T("Remove"),												_T("�Ƴ��о�"),									//810
	_T("Onload 1st Slot:"),										_T("���ϵ�һ�ۣ�"),								//811
	_T("Slot Pitch:"),											_T("�ۼ�ࣺ"),									//812
	_T("Slot Position Fine Tune"),								_T("�ۼ��΢��"),								//813
	_T("Swap Pitch:"),											_T("���Ϻм�ࣺ"),								//814
	_T("Next Slot"),											_T("��һ��"),									//815
	_T("Indexer Alignment"),									_T("ץƬ��λ��У׼"),							//816
	_T("Indexer Right Limit"),									_T("ץƬ���Ҽ���"),								//817
	_T("Indexer Logic Zero"),									_T("ץƬ���߼����"),							//818
	_T("Index Tune"),											_T("����ƫ��������"),							//819
	_T("Holding Fork"),											_T("�����ϲ�"),									//820
	_T("Pully Motor"),											_T("�ƶ���"),									//821
	_T("Taught Pos"),											_T("ѧϰλ��"),									//822
	_T("Left Claw:"),											_T("����Ȧ:"),									//823
	_T("Right Claw:"),											_T("����Ȧ:"),									//824
	_T("Puller:"),												_T("��Ƭ��:"),									//825
	_T("Onload Air:"),											_T("��������:"),								//826
	_T("Offload Air:"),											_T("��������:"),								//827
	_T("Without LF"),											_T("��֧�ܿ���"),								//828
	_T("Solenoid Test"),										_T("���߹ܲ���"),								//829
	_T("Please remove all magazines on elevator"),				_T("���Ƴ��������������Ϻ�"),					//830
	_T("Please wait elevator home and standby finish"),			_T("��ȴ�����������λ"),						//831
	_T("Please adjust first slot level"),						_T("������Ϻе�һ��λ��"),						//832
	_T("Please adjust last slot level"),						_T("������Ϻ����һ��λ��"),					//833
	_T("Please adjust 1st slot level of next magazine"),		_T("����ڵڶ��Ϻе�һ��λ��"),					//834
	_T("Operating side: Onloader"),								_T("������������������"),						//835
	_T("Operating side: Offloader"),							_T("������������������"),						//836
	_T("Press 'Confirm' to load recipe, 'Stop' to cancel"),		_T("����ȷ�ϡ����س�ʽ������ֹͣ��ȡ��"),			//837	//V1.1.W141 edit
	_T("Press 'Done' to quit"),									_T("��'���'�˳�"),								//838	//v3.0.T454 edit
	_T("Please wait while indexer homing..."),					_T("��ȴ�ץƬ������..."),						//839
	_T("Please move indexer to Logic Zero position"),			_T("���ƶ�ץƬ����ʹ���е��׼������Ŀ̱꣡"),	//840
	_T("Press Next to search the Right Limit Sensor!"),			_T("��'��һ��'ץƬ���Զ���Ѱ�Ҽ��ޣ�"),			//841	//V1.1.W139 edit
	_T("Press Next to enter Indexing Offset Adjustment!"),		_T("��'��һ��'����ѧϰ����ƫ������"),			//842	//V1.1.W139 edit
	_T("Please ensure no leadframe on track!"),					_T("��ȷ���������֧�ܣ�"),						//843
	_T("Press 'Next' to index, then tune position"),			_T("��'��һ��'����֧�ܣ�Ȼ�����λ��"),			//844	//V1.1.W139 edit
	_T("Onload Elevator No Magazine!"),							_T("��������̨���Ϻ�"),							//845	//V1.1.W136 edit
	_T("Offload Elevator No Magazine!"),						_T("��������̨���Ϻ�"),							//846	//V1.1.W136 edit
	_T("Search Location Time Out!"),							_T("����λ�ô�������ʱ��"),						//847
	_T("Leadframe Jamed the Offload Elevator!"),				_T("���Ͽ��ϣ�"),								//848
	_T("Onload Elevator No Response!"),							_T("��������̨����Ӧ��"),						//849	//V1.1.W136 edit
	_T("Calibration"),											_T("У׼"),										//850
	_T("Verification"),											_T("����"),										//851
	_T("Tail Break"),											_T("��β����"),									//852
	_T("Distance (um)"),										_T("����(um)"),									//853
	_T("Eject"),												_T("�ͳ��Ϻ�"),									//854	//V1.1.W149 edit
	_T("Edit Material Handler Program"),						_T("�༭����ϵͳ��ʽ"),							//855
	_T("Claw Offset (um)"),										_T("����λ��΢��(um)"),							//856
	_T("Delete all elevator data and re-teach?"),				_T("ȷ��ɾ����������̨���ݣ�"),					//857	//V1.1.W136 edit
	_T("Delete all PR index data and re-teach? "),				_T("ȷ��ɾ�����н���ͼ�����ݣ�"),				//858
	_T("Please Manually Remove Leadframe On Track!"),			_T("���ֶ��Ƴ������ϵĲ��ϣ�"),					//859
	_T("Remove Leadframe On Track Automatically ?"),			_T("�Ƿ��Զ��Ƴ������ϵĲ��ϣ�"),				//860
	_T("Open Clamp Delay (ms)"),								_T("�оߴ���ʱ(ms)"),							//861
	_T("Leadframe left on the track last time!"),				_T("��⵽�ϴιػ�ǰ�������δ���֧��"),		//862
	_T("Continue to bond remaining leadframe on track? "),		_T("�Ƿ�����ϴ�δ���֧�ܣ�"),					//863
	_T("Enhance"),												_T("��ǿ"),										//864
	_T("No Lf Run"),											_T("��֧�ܿ���"),								//865
	_T("Teach Material Handler Program"),						_T("ѧϰ����ϵͳ��ʽ"),							//866
	_T("Move table to safe position, then press Next"),			_T("�ƶ�����̨����ȫλ��, Ȼ��'��һ��'"),		//867	//V1.1.W139 edit
	_T("Teach Table Safe Position"),							_T("ѧϰXY��ȫλ��"),							//868
	_T("Program Exist"),										_T("���г�ʽ"),									//869
	_T("No Recipe"),											_T("�޳�ʽ"),									//870
	_T("Teach New"),											_T("�³�ʽ"),									//871
	_T("Teach Bond Program"),									_T("ѧϰ���߳�ʽ"),								//872
	_T("Edit Bond Program "),									_T("�༭���߳�ʽ"),								//873
	_T("1st LF Preheat Time (ms):"),							_T("��һ��֧��Ԥ��ʱ��(ms):"),					//874
	_T("Normal LF Preheat Time (ms):"),							_T("����֧��Ԥ��ʱ��(ms):"),					//875
	_T("Offload Position (1/2):"),								_T("����λ��(1/2):"),							//876
	_T("Position1"),											_T("λ��1"),									//877
	_T("Position2"),											_T("λ��2"),									//878
	_T("Advanced Vision"),										_T("�߼��Ӿ�"),									//879
	_T("Rotate Angle:"),										_T("��ת�Ƕ�:"),								//880
	_T("Shape Recognize"),										_T("��״ʶ��"),									//881
	_T("Impact Force 1"),										_T("���ѹ��һ"),								//882
	_T("Impact Time 1"),										_T("���ʱ��һ"),								//883
	_T("Impact Time 2"),										_T("���ʱ���"),								//884
	_T("Impact Power 1"),										_T("�������һ"),								//885
	_T("Impact Power 2"),										_T("���������"),								//886
	_T("Machine is in Standby Mode..."),						_T("����˯��ģʽ��..."),						//887
	_T("Click Mouse Left Button to Quit Standby Mode!"),		_T("�����������˳�˯��ģʽ��"),				//888
	_T("Enter Machine Standby Mode"),							_T("�������˯��ģʽ"),							//889
	_T("Back-up ID:"),											_T("����ģ��ţ�"),								//890
	_T("Arrange the area of BACK-UP PR pattern"),				_T("ѧϰ����ͼ��ģ��"),							//891
	_T("Press Next to finish"),									_T("��'��һ��'����ѧϰ"),						//892	//V1.1.W139 edit
	_T("Press Next to adjust leadframe position"),				_T("��'��һ��'����֧��λ��"),					//893	//V1.1.W139 edit
	_T("Setup jig, then press capillary to surface"),			_T("�밲װУ׼���ߣ���ѹ��ͷ�������"),			//894
	_T("Press Next to record Flat Level"),						_T("��'��һ��'��¼��ͷˮƽλ"),					//895	//V1.1.W139 edit
	_T("Release bond head, then remove jig"),					_T("�ɿ���ͷȻ���Ƴ�У׼����"),					//896
	_T("Flat Level"),											_T("��ͷˮƽλ"),								//897
	_T("Multiple BTOS"),										_T("��ɾ�ƫ��"),								//898
	_T("Confirm Red Button on the panel is  ON !"),				_T("��ȷ�Ϻ�ɫ��￪�ؿ�����"),					//899
	_T("Ultra Low"),											_T("����"),										//900	//V1.1.W149 edit
	_T("Pre Impact Time (ms)"),									_T("ǰ��Ԥ��ʱ�� (ms)"),						//901	//V1.1.W149 edit
	_T("Pre Shift 2 (um)"),										_T("Ԥ���ƶ� (um)"),							//902
	_T("Are you sure to overwrite current device setting?"),	_T("ȷ�����Ĳ��ϲ�����"),						//903
	_T("Please make sure the vision is not expired"),			_T("��������ʹ��Ȩ���Ƿ���ڣ�"),				//904
	_T("Debug"),												_T("����"),										//905
	_T("N2H2 Enable"),											_T("��������"),									//906
	_T("N2H2 Stable Time"),										_T("�����ȶ�ʱ��"),								//907
	_T("Waiting N2H2 stable..."),								_T("�ȴ�������..."),							//908
	_T("Onload Magazine Pully full"),							_T("����������������Ϻ�"),						//909
	_T("Offload Magazine Pully full"),							_T("����������������Ϻ�"),						//910
	_T("E-Torch Burn In"),										_T("����������"),								//911
	_T("Cycle Time"),											_T("����ʱ��"),									//912	//v1.5.T71 edit
	_T("No. of Cycle"),											_T("���ڴ���"),									//913	//v1.5.T71 edit
	_T("Are you sure to delete all back-up PR?"),				_T("ȷ��ɾ�����б���ģ�壿"),					//914
	_T("Are you sure to swap back-up PR to main PR?"),			_T("ȷ�Ͻ���ǰģ��������ģ�壿"),				//915
	_T("Swap"),													_T("����"),										//916
	_T("Clean All"),											_T("ȫɾ��"),									//917
	_T("Auto Swap Main PR"),									_T("�Զ�������ģ��"),							//918
	_T("Multi-PR"),												_T("���ʶ��"),									//919
	_T("Adjust Lead BTOS"),										_T("���ڹܽŴɾ�ƫ��"),							//920
	_T("Capillary Wear Out Power Compensation"),				_T("�ɾ�ĥ����������"),							//921
	_T("Start/End Count"),										_T("�ɾ׿�ʼ/��������"),						//922
	_T("Power Comp 1/2 (%)"),									_T("�������� 1/2 (%)"),							//923
	_T("FAB2 Tail Length"),										_T("FAB2 ��β����"),							//924
	_T("BSOB FAB Type"),										_T("���� FAB ���"),							//925	//v3.1.T381 rename
	_T("Horizon"),												_T("����"),										//926
	_T("Circle"),												_T("Բ��"),										//927
	_T("Ball Scrub"),											_T("������ĥ"),									//928
	_T("Contrast"),												_T("�Աȶ�"),									//929
	_T("Red Gain"),												_T("������"),									//930
	_T("Brightness"),											_T("����"),										//931
	_T("Saturation"),											_T("���Ͷ�"),									//932
	_T("PR Filter"),											_T("ͼ���˾�"),									//933
	_T("Press 'Confirm' to reset, press 'Stop' to cancel"),		_T("����ȷ�ϡ����������ֹͣ��ȡ��"),				//934	//V1.1.W261 edit
	_T("Warning On Serial Skip Num"),							_T("��������������Ŀ"),							//935	//V1.1.W262 edit
	_T("Tail Bond"),											_T("β��"),										//936
	_T("Bond Tools"),											_T("���߹���"),									//937
	_T("Capi Smooth"),											_T("�ɾ�ĥ��"),									//938
	_T("Smooth"),												_T("ĥ��"),										//939
	_T("Clean"),												_T("���"),										//940
	_T("Press 'Start' to home indexer"),						_T("��'��ʼ'����ץƬ��"),						//941
	_T("Press 'Start' to teach Index PR"),						_T("��'��ʼ'ѧϰ����ͼ��"),						//942
	_T("Press 'Teach New' to index a new leadframe"),			_T("��'�³�ʽ'��һƬ��֧��"),					//943
	_T("Press 'Done' to confirm final position"),				_T("��'���'ȷ�����λ��"),						//944
	_T("Press 'Start' to teach elevator position"),				_T("��'��ʼ'ѧϰ����̨λ��"),					//945	//V1.1.W136 edit
	_T("Press 'Next' to skip this step"),						_T("��'��һ��'�����˲�"),						//946	//V1.1.W139 edit
	_T("Incomplete Rotate Compensation:"),						_T("��צ��תλ�ò�����"),						//947
	_T("Press 'Continue' to preview bond sequence"),			_T("��'����'Ԥ������˳��"),						//948
	_T("BBOS Parameter"),										_T("ѹ�����"),									//949
	_T("Hint: If multiple dies in one cup, please input numbers above��"),	_T("��ʾ�����һ�������ж��оƬ������������������"),	//950
	_T("Hint: Found multiple die in one cup"),					_T("��ʾ������һ�������ж��оƬ"),				//951
	_T("Choose correct die on top combo box"),					_T("�������Ϸ���������ѡ�񺸵��ӦоƬ"),		//952	//V1.1.W149 edit
	_T("Self-define"),											_T("���߷�Χ"),									//953
	_T("Input Bond Start Wire and End Wire Number"),			_T("���뺸�ӿ�ʼ�������ߺ�"),					//954
	_T("Start Wire:"),											_T("��ʼ�ߺţ�"),								//955
	_T("End Wire:"),											_T("�����ߺţ�"),								//956
	_T("Bump Wire Bond Power"),									_T("�����ߺ�����"),								//957
	_T("Onloader Push Kicker"),									_T("������Ƭ��"),								//958
	_T("Offloader Time Out!"),									_T("����������̨��ʱ"),							//959	//V1.1.W136 edit
	_T("Are You Sure To Delete Original Recipe"),				_T("��ȷ��Ҫ���ԭʼ�ĳ���"),					//960
	_T("And Teach New Recipe?"),								_T("�����µĳ���?"),							//961
	_T("Edit Point"),											_T("����༭"),									//962
	_T("Looping Pre-Trigger"),									_T("�߻���ǰ��"),								//963
	_T("Wire Clamp Pre-Trigger"),								_T("�߼���ǰ��"),								//964
	_T("Tail Break Threshold:"),								_T("��β������ֵ:"),							//965
	_T("Pre Impact Power 1"),									_T("���Ԥ������һ"),							//966
	_T("Pre Impact Power 2"),									_T("���Ԥ��������"),							//967
	_T("BSOB Direction Angle (Deg)"),							_T("������Ƕ� (��)"),						//968
	_T("Flex"),													_T("����"),										//969
	_T("BSOB Bump Shape Mode"),									_T("��������ģʽ"),								//970
	_T("Reverse Motion (um)"),									_T("�����˶����(um)"),							//971
	_T("Reverse Angle (Deg)"),									_T("�����˶��Ƕ�(��)"),							//972
	_T("Loop Height (um)"),										_T("�߻��߶�(um)"),								//973
	_T("Stick Detection"),										_T("��ճ���"),									//974
	_T("BH is not in the safe position"),						_T("��ͷδ���ڰ�ȫλ��"),						//975
	_T("Wait on/off loader finish..."),							_T("��ȴ����������������..."),				//976
	_T("Wire Feed Error, Please Check Bond Quality!"),			_T("���ߴ���, ���麸������!"),				//977
	_T("Fast Tail"),											_T("������β����"),								//978
	_T("Wire Feed Check"),										_T("���߼��"),									//979
	_T("L Angle"),												_T("Lƫת�Ƕ�"),								//980
	_T("Index PR Delay (ms)"),									_T("�о�Ԥ�н���ʱ(ms)"),						//981
	_T("Tail Length Monitor"),									_T("��β���ȼ��"),								//982
	_T("Coarse Detection:"),									_T("��ɢ���:"),								//983
	_T("Coarse Detection Samples:"),							_T("��ɢ�������:"),							//984
	_T("Matrix Die Ahead"),										_T("����оƬԤ��λ"),							//985
	_T("Matrix Row/Col"),										_T("��������/����"),							//986	//v1.5.T91 edit
	_T("Move table to matrix 2nd-col top-right align pt"),		_T("�ƶ�����̨������ڶ������ϽǶ�λ��"),		//987
	_T("Move table to matrix 2nd-row top-right align pt"),		_T("�ƶ�����̨������ڶ������ϽǶ�λ��"),		//988
	_T("Unit Row/Col number wrong!"),							_T("��Ԫ��/��������!"),							//989
	_T("Matrix Row/Col number wrong!"),							_T("������/��������!"),							//990
	_T("Teach Matrix Pitch:"),									_T("ָ��������:"),							//991
	_T("Power"),												_T("����"),										//992
	_T("Time"),													_T("ʱ��"),										//993
	_T("Impact Power (%)"),										_T("�������(%)"),								//994
	_T("Bond Power (%)"),										_T("��������(%)"),								//995
	_T("Post Damp Power (%)"),									_T("��������(%)"),								//996	//V1.1.W149 edit
	_T("Teach Pad Size"),										_T("ָ�����̳ߴ�"),								//997
	_T("Step1:"),												_T("����1:"),									//998
	_T("Move cross hair to top-left point of pad"),				_T("�ƶ���굽���̵����Ͻ�"),					//999
	_T("Press Enter to continue"),								_T("��'ȷ��'����"),								//1000
	_T("Press ESC to cancel"),									_T("��'�˳�'ȡ��"),								//1001
	_T("Esc"),													_T("ȡ��"),										//1002
	_T("Enter"),												_T("ȷ��"),										//1003
	_T("Step2:"),												_T("����2:"),									//1004
	_T("Step3:"),												_T("����3:"),									//1005
	_T("Move cross hair to top-right point of pad"),			_T("�ƶ���굽���̵����Ͻ�"),					//1006
	_T("Move cross hair to bottom-right point of pad"),			_T("�ƶ���굽���̵����½�"),					//1007
	_T("Edit Pad Size"),										_T("�༭���̳ߴ�"),								//1008
	_T("Step4:"),												_T("����4:"),									//1009
	_T("Teach Bond Points"),									_T("ѧϰ����λ��"),								//1010
	_T("Move to group 1st point"),								_T("�ƶ���굽С��ĵ�һ����"),					//1011
	_T("Move to group 2nd point"),								_T("�ƶ���굽С��ĵڶ�����"),					//1012
	_T("Move to group last point"),								_T("�ƶ���굽С������һ����"),				//1013
	_T("Wire Length Control"),									_T("�߳�����"),									//1014
	_T("Constant Wire Length"),									_T("�̶��߳�"),									//1015
	_T("Wire Length"),											_T("�߳�"),										//1016
	_T("0: No; 1: Yes"),										_T("0:��; 1:��"),								//1017
	_T("Length:"),												_T("֧�ܳ���"),									//1018
	_T("Confirm no LF between track and elevators"),			_T("��ȷ�Ϲ��������̨֮����֧��"),				//1019	//V1.1.W136 edit
	_T("Confirm no magazine under elevators"),					_T("��ȷ������̨�·����ϰ�"),					//1020	//V1.1.W136 edit
	_T("Pad Pattern Manager"),									_T("����ģ�����"),								//1021
	_T("Pad Patterns"),											_T("����ģ��"),									//1022
	_T("Current"),												_T("��ǰ"),										//1023
	_T("UPH Mode"),												_T("UPHģʽ"),									//1024
	_T("Ref"),													_T("��λ"),										//1025
	_T("Pad Type"),												_T("��������"),									//1026
	_T("Teach Bond Point"),										_T("ָ������"),									//1027
	_T("Taught(1/2)"),											_T("�ѽ�(1/2)"),								//1028
	_T("Single"),												_T("����"),										//1029
	_T("Group"),												_T("��"),										//1030
	_T("Intelli Pad Locator"),									_T("���ܺ��̶�λ"),								//1031
	_T("Bond On Property"),										_T("��������"),									//1032
	_T("1st Bond Point Pad ID"),								_T("һ���㺸������"),							//1033
	_T("1st Bond Point X Offset"),								_T("һ����Xƫ��"),								//1034
	_T("1st Bond Point Y Offset"),								_T("һ����Yƫ��"),								//1035
	_T("Pad locating fail!"),									_T("���̶�λʧ��!"),							//1036
	_T("Please manually locate pad"),							_T("���ֶ���λ"),								//1037
	_T("Pad locating fail! Pls select pad ID."),				_T("���̶�λʧ��!"),							//1038
	_T("Then manually locate pad."),							_T("��ѡ���������ֶ���λ."),					//1039
	_T("1st and 2nd bond point number not match"),				_T("һ��������������ƥ��"),						//1040
	_T("Capillary"),											_T("�����ɾ�"),									//1041
	_T("Change Capillary"),										_T("�����ɾ�"),									//1042
	_T("Remove old capillary"),									_T("����ɴɾ�"),								//1043
	_T("Insert new capillary"),									_T("��װ�´ɾ�"),								//1044
	_T("Insert capillary jig under transducer"),				_T("���ɾ׹氲װ�ڻ������·�"),					//1045
	_T("Tighten capillary with proper torque"),					_T("����ȷ��Ť�������ɾ�"),						//1046
	_T("Remove capillary jig"),									_T("����ɾ׹�"),								//1047
	_T("Please check USG impedance"),							_T("���鳬���迹"),							//1048
	_T("Step5:"),												_T("����5:"),									//1049
	_T("Step6:"),												_T("����6:"),									//1050
	_T("Step7:"),												_T("����7:"),									//1051
	_T("Power On"),												_T("�ϵ�"),										//1052
	_T("Power Off"),											_T("�ϵ�"),										//1053
	_T("Teach Dummy Bond Area"),								_T("ѧϰ��β����"),								//1054
	_T("Move cross hair to top-left point"),					_T("�ƶ���굽���Ͻ�"),							//1055
	_T("Move cross hair to bottom-right point"),				_T("�ƶ���굽���½�"),							//1056
	_T("Press Continue to learn contact level"),				_T("��'����'ѧϰ�Ӵ���߶�"),					//1057
	_T("Teach"),												_T("ѧϰ"),										//1058
	_T("Bond Area"),											_T("��βλ��"),									//1059
	_T("Area Teach Status"),									_T("��βλ��ѧϰ״̬"),							//1060
	_T("Swap Wire"),											_T("�������"),									//1061
	_T("First Wire"),											_T("��һ����"),									//1062
	_T("Second Wire"),											_T("�ڶ�����"),									//1063
	_T("Wire Length Correction"),								_T("�߳�����"),									//1064
	_T("1st Bond Parameter"),									_T("һ������"),									//1065
	_T("2nd Bond Parameter"),									_T("��������"),									//1066
	_T("IPL Delay (ms)"),										_T("IPL��ʱ (ms)"),								//1067
	_T("Pre Kink"),												_T("Ԥ�۵�"),									//1068
	_T("Pre Kink Height (um)"),									_T("Ԥ�۵�߶� (um)"),							//1069
	_T("Pre Kink Dist (um)"),									_T("Ԥ�۵㳤�� (um)"),							//1070
	_T("Pre Kink Angle (Deg)"),									_T("Ԥ�۵�Ƕ� (��)"),							//1071
	_T("1st Pre-heat Delay (ms)"),								_T("��ƬԤ���� (ms)"),							//1072
	_T("EFO Test"),												_T("������"),									//1073	//v1.5.T71 edit
	_T(""),														_T(""),											//1074
	_T("Please convert to Single Unit first!"),					_T("����ת��Ϊ��һ��Ԫ!"),						//1075
	_T("Single Unit"),											_T("��һ��Ԫ"),									//1076
	_T("Group Wire"),											_T("���߷���"),									//1077
	_T("Apply To:"),											_T("Ӧ����:"),									//1078
	_T("Vacuum Control"),										_T("��տ���"),									//1079
	_T("Vacuum Pressure Too Low!"),								_T("�����ѹ����!"),							//1080
	_T("Vacuum:"),												_T("���:"),									//1081
	_T("Please select Program Type"),							_T("��ѡ���������"),							//1082	//v1.5.T91 edit
	_T("2nd Bond Point Pad ID"),								_T("�����㺸������"),							//1083
	_T("2nd Bond Point X Offset"),								_T("������Xƫ��"),								//1084
	_T("2nd Bond Point Y Offset"),								_T("������Yƫ��"),								//1085
	_T("1st Bond Scrub"),										_T("һ����ĥ"),									//1086
	_T("2nd Bond Scrub"),										_T("������ĥ"),									//1087
	_T("Bump Ball Scrub"),										_T("������ĥ"),									//1088
	_T("If need to change program,"),							_T("���Ҫ�޸ĳ���,"),							//1089
	_T("quit this menu, then edit Manual and PR Align."),		_T("��ӵ�ǰ�˵��˳�,�༭�ֶ����Զ���λ�㡣"),	//1090
	_T("If no need to change program,"),						_T("������޸ĳ���,"),							//1091
	_T("X Motor overheat"),										_T("X������"),								//1092
	_T("Y Motor overheat"),										_T("Y������"),								//1093
	_T("EFO"),													_T("EFO"),										//1094
	_T("Spark Configuration"),									_T("������"),									//1095
	_T("Gap Large DAC"),										_T("������DAC"),								//1096
	_T("Length"),												_T("�߳�"),										//1097
	_T("CD"),													_T("CD"),										//1098
	_T("Profile"),												_T("����"),										//1099
	_T("Neck"),													_T("�߾�"),										//1100
	_T("Kink2"),												_T("�յ�2"),									//1101	//v3.1.T320 edit
	_T("Kink3"),												_T("�յ�3"),									//1102	//v3.1.T320 edit
	_T("Twist"),												_T("Ť��"),										//1103
	_T("Landing"),												_T("��½"),										//1104
	_T("Kink3 Span Length"),									_T("�յ�3���"),								//1105	//v3.1.T320 edit
	_T("Kink3 Span Length Unit"),								_T("�յ�3��ȵ�λ"),							//1106	//v3.1.T320 edit
	_T("Kink3 Height Factor (%)"),								_T("�յ�3�߶ȱ��� (%)"),						//1107	//v3.1.T320 edit
	_T("Kink3 Smoothness"),										_T("Kink3 Smoothness"),							//1108
	_T("Slope Length (um)"),									_T("Slope Length (um)"),						//1109
	_T("Contact Angle Correction (Degree)"),					_T("Contact Angle Correction (Degree)"),		//1110
	_T("Twist Ratio (%)"),										_T("Twist Ratio (%)"),							//1111
	_T("Are you sure to change loop profile?"),					_T("ȷ��Ҫ�Ļ��߻���״��"),					//1112
	_T("Are you sure to change loop type?"),					_T("ȷ��Ҫ�Ļ��߻�������"),					//1113
	_T("Neck Wire Feed Speed (%)"),								_T("�߾������ٶ� (%)"),							//1114
	_T("Neck Wire Bent Speed (%)"),								_T("�߾������ٶ� (%)"),							//1115
	_T("Pre Kink Wire Feed Speed (%)"),							_T("Ԥ�۵�����ٶ� (%)"),						//1116
	_T("Pre Kink Wire Bent Speed (%)"),							_T("Ԥ�۵������ٶ� (%)"),						//1117
	_T("Kink2 Wire Feed Speed (%)"),							_T("�յ�2�����ٶ� (%)"),						//1118	//v3.1.T320 edit
	_T("Kink2 Wire Bent Speed (%)"),							_T("�յ�2�����ٶ� (%)"),						//1119	//v3.1.T320 edit
	_T("Kink3 Wire Feed Speed (%)"),							_T("�յ�3�����ٶ� (%)"),						//1120	//v3.1.T320 edit
	_T("Kink3 Wire Bent Speed (%)"),							_T("�յ�3�����ٶ� (%)"),						//1121	//v3.1.T320 edit
	_T("Twist Wire Feed Speed (%)"),							_T("Ť������ٶ� (%)"),							//1122
	_T("Twist Reverse Bent Speed (%)"),							_T("Ť�䷴�������ٶ� (%)"),						//1123
	_T("Twist Forward Bent Speed (%)"),							_T("Ť�����������ٶ� (%)"),						//1124
	_T("Trajectory Speed (%)"),									_T("�߻��켣�ٶ� (%)"),							//1125
	_T("Pull Speed (%)"),										_T("�����ٶ� (%)"),								//1126
	_T("Kink3 Height Pre-Move (%)"),							_T("�յ�3�߶���ǰ�ƶ� (%)"),					//1127	//v3.1.T320 edit
	_T("Kink3 Distance Pre-Move (%)"),							_T("�յ�3������ǰ�ƶ� (%)"),					//1128	//v3.1.T320 edit
	_T("Reverse Height 4 Pre-Move (%)"),						_T("����߶�4��ǰ�ƶ� (%)"),					//1129
	_T("Twist Pre-Move (%)"),									_T("Ť����ǰ�ƶ� (%)"),							//1130
	_T("Remove Magazines ON the Elevator"),						_T("���Ƴ�����̨���ϺУ�"),						//1131
	_T("Remove Magazines BELOW the Elevator"),					_T("���Ƴ�����̨���ϺУ�"),						//1132
	_T("Lead Tip Offset (um)"),									_T("���Ŷ���ƫ��(um)"),							//1133
	_T("ILL Delay (ms)"),										_T("ILL��ʱ(ms)"),								//1134
	_T("Lead Capillary Offset (um)"),							_T("���Ŵɾ�ƫ��(um)"),							//1135
	_T("Intelli Lead Locator"),									_T("�������Ŷ�λ"),								//1136
	_T("1st Bond Point ILL"),									_T("һ����ILL"),								//1137
	_T("1st Bond Point Lead Capillary Offset"),					_T("һ�������Ŵɾ�ƫ��"),						//1138
	_T("1st Bond Point Lead X Offset"),							_T("һ��������Xƫ��"),							//1139
	_T("1st Bond Point Lead Y Offset"),							_T("һ��������Yƫ��"),							//1140
	_T("2nd Bond Point ILL"),									_T("������ILL"),								//1141
	_T("2nd Bond Point Lead Capillary Offset"),					_T("���������Ŵɾ�ƫ��"),						//1142
	_T("2nd Bond Point Lead X Offset"),							_T("����������Xƫ��"),							//1143
	_T("2nd Bond Point Lead Y Offset"),							_T("����������Yƫ��"),							//1144
	_T("BSOB Cut Power (Dac)"),									_T("������������ (Dac)"),						//1145
	_T("BSOB Cut Power Delay (%)"),								_T("��������������ʱ (%)"),						//1146
	_T("Matrix Hybrid"),										_T("����ϳ�"),									//1147
	_T("Lead locating fail!"),									_T("���Ŷ�λʧ��!"),							//1148
	_T("Please manually locate lead"),							_T("���ֶ���λ����"),							//1149
	_T("Auto"),													_T("�Զ�"),										//1150
	_T("Spark Open!"),											_T("���ʧ�ܣ�"),								//1151
	_T("Spark Gap Large!"),										_T("��������"),							//1152
	_T("1st Unit Heat Delay(ms)"),								_T("�׵�Ԫ������ʱ(ms)"),						//1153
	_T("Calibration In Progress..."),							_T("У׼������..."),							//1154
	_T("USG Calibration"),										_T("������У׼"),								//1155
	_T("Stepper Control"),										_T("����������"),								//1156
	_T("Capillary Auto Unplug"),								_T("�Զ�ͨ�ɾ�"),								//1157
	_T("Unplug"),												_T("ͨ�ɾ�"),									//1158
	_T("Please Teach Dummy Bond Area!"),						_T("����ѧϰ��β����!"),						//1159
	_T("Capillary Unplug Failure!"),							_T("ͨ�ɾ�ʧ��!"),								//1160
	_T("Thread Wire"),											_T("����"),										//1161
	_T("Lead Frame Holder:"),									_T("֧�ܱ�����:"),								//1162
	_T("Matrix Information"),									_T("������Ϣ"),									//1163
	_T("Matrix Row:"),											_T("��������:"),								//1164
	_T("Matrix Column:"),										_T("��������:"),								//1165
	_T("Current Matrix:"),										_T("��ǰ����:"),								//1166
	_T("Matrix Bonding Schedule"),								_T("�����߼ƻ�"),								//1167
	_T("Matrix Layout"),										_T("���󷢲�"),									//1168
	_T("Index Time"),											_T("�������"),									//1169
	_T("Total Row:"),											_T("������"),									//1170
	_T("Total Column:"),										_T("������"),									//1171
	_T("Current Row"),											_T("��ǰ��"),									//1172
	_T("Current Column"),										_T("��ǰ��"),									//1173
	_T("Reschedule"),											_T("���żƻ�"),									//1174
	_T("Width:"),												_T("֧�ܿ��:"),								//1175
	_T("Mag Width:"),											_T("�Ϻп��:"),								//1176
	_T("Mag Length:"),											_T("�Ϻг���:"),								//1177
	_T("Device Offset"),										_T("�豸����"),									//1178
	_T("Fork Detected on OnLoader"),							_T("�����ϲ���! "),								//1179
	_T("Magazine Drop Detected on Offloader"),					_T("�����Ϻ���!"),								//1180
	_T("Magazine Drop Detected on Onloader"),					_T("�����Ϻ���!"),								//1181
	_T("Magazine Clamper"),										_T("�Ϻмо�"),									//1182
	_T("Material Handler"),										_T("����ϵͳ"),									//1183
	_T("Clamp Pos"),											_T("�н�λ��"),									//1184
	_T("Onload Magazine Buffer Empty"),							_T("�����Ϻ��ѿ�"),								//1185
	_T("Offload Magazine Buffer Empty"),						_T("�����Ϻ��ѿ�"),								//1186
	_T("Offload Output Jam"),									_T("�����Ϻп�ס"),								//1187
	_T("Narrow"),												_T("խ"),										//1188
	_T("Wide"),													_T("��"),										//1189
	_T("Please Check Rear Rail Type"),							_T("����󵼹�����"),							//1190
	_T("Wire Feed Count"),										_T("��������"),									//1191
	_T("Fork Detected on Offloader"),							_T("�����ϲ���!"),								//1192
	_T("BSOB Offset"),											_T("�����߲���"),								//1193
	_T("BSOB Pre Impact Force 1"),								_T("����Ԥ��ѹ��һ"),							//1194
	_T("BSOB Pre Impact Time 1"),								_T("����Ԥ��ʱ��һ"),							//1195
	_T("BSOB Pre Impact Power 1"),								_T("����Ԥ������һ"),							//1196
	_T("BSOB Time 1"),											_T("����ʱ��һ"),								//1197
	_T("BSOB Time 2"),											_T("����ʱ���"),								//1198
	_T("BSOB Power 1"),											_T("��������һ"),								//1199
	_T("BSOB Power 2"),											_T("����������"),								//1200
	_T("BSOB Force 1"),											_T("����ѹ��һ"),								//1201
	_T("BSOB Force 2"),											_T("����ѹ����"),								//1202
	_T("BSOB Impact Time 1"),									_T("������ʱ��һ"),							//1203
	_T("BSOB Impact Time 2"),									_T("������ʱ���"),							//1204
	_T("BSOB Impact Power 1"),									_T("����������һ"),							//1205
	_T("BSOB Impact Power 2"),									_T("������������"),							//1206
	_T("BSOB Impact Force 1"),									_T("������ѹ��һ"),							//1207
	_T("BSOB Impact Force 2"),									_T("������ѹ����"),							//1208
	_T("1st Bump Parameter"),									_T("һ������"),									//1209
	_T("2nd Bump Parameter"),									_T("��������"),									//1210
	_T("Wire Length Exceed 8mm"),								_T("�߳����� 8mm"),								//1211
	_T("Col"),													_T("��"),										//1212
	_T("Ref1"),													_T("��λһ"),									//1213
	_T("Ref2"),													_T("��λ��"),									//1214
	_T("Bump Shape Parameter"),									_T("�������β���"),								//1215
	_T("Light"),												_T("����"),										//1216
	_T("Please check Onload Y1 Sensor Plate"),					_T("��� ���Ϻ����ﴫ������"),				//1217 
	_T("Please check Onload Y2 Sensor Plate"),					_T("��� ����ǰ����ﴫ������"),				//1218 
	_T("Please check Offload Y1 Sensor Plate"),					_T("��� ���Ϻ���ﴫ������"),					//1219 
	_T("Please check Offload Y2 Sensor Plate"),					_T("��� ����ǰ����ﴫ������"),				//1220 
	_T("Please check Front Rail Sensor Plate"),					_T("��� ǰ������ﴫ��������"),				//1221 
	_T("Please check Rear Rail Sensor Plate"),					_T("��� �󵼹���ﴫ������"),					//1222 
	_T("Please check lead frame at Onload "),					_T("��� Onload lead frame"),					//1223
	_T("Motor Out of limt, cannot be moved "),					_T("��ﳬ��������Χ"),							//1224 
	_T("Side Clamper"),											_T("��о�"),									//1225 
	_T("Front Clamper"),										_T("ǰ�о�"),									//1226 
	_T("Rear Clamper"),											_T("��о�"),									//1227 
	_T("Acc Ratio:"),											_T("���ٱ�ֵ:"),								//1228 
	_T("Front Rail"),											_T("ǰ����"),									//1229
	_T("Rear Rail "),											_T("�󵼹�"),									//1230
	_T("Current Depth"),										_T("��ǰ���:"),								//1231
	_T("Ejector"),												_T("����ץƬ"),									//1232
	_T("Move Distance"),										_T("�ƶ�����"),									//1233
	_T("Distance to Home"),										_T("��λ����������"),							//1234
	_T("Front & Rear Clamper"),									_T("ǰ��о�"),									//1235
	_T("Injector Problem"),										_T("������ʧ��"),								//1236
	_T("Shift"),												_T("ƽ��"),										//1237
	_T("Width"),												_T("���"),										//1238
	_T("Please check Window Clamper Sensor Plate"),				_T("��� �о���ﴫ������"),					//1239
	_T("Please close other control dialog first"),				_T("��ر�����������ﴰ��"),					//1240
	_T("Save"),													_T("����"),										//1241
	_T("Machine Offset"),										_T("��������"),									//1242
	_T("Inject"),												_T("�ƽ���Ԫ"),									//1243
	_T("Search"),												_T("������Ԫ"),									//1244
	_T("Compensate"),											_T("������Ԫ"),									//1245
	_T("Catch Up"),												_T("׷�ϵ�Ԫ"),									//1246	
	_T("half Index"),											_T("��������Ԫ"),								//1247
	_T("Slot"),													_T("�Ϻ��ܲ���:"),								//1248
	_T("Yes"),													_T("ͬ��"),										//1249
	_T("No"),													_T("��ͬ��"),									//1250
	_T("Height"),												_T("�߶�"),										//1251  
	_T("Self-define "),											_T("�Զ���"),									//1252
	_T("Cannot load from ID_CUELITE.ini "),						_T("�����Դ� ID_CUELITE.ini ����"),				//1253  
	_T("Upload by calculation ? "),								_T("ʹ�ü������� ?"),							//1254  
	_T("Save Machine Offset to ID_CUELITE.ini"),				_T("������������� ID_CUELITE.ini"),			//1255
	_T("Injector"),												_T("����ץƬ"),									//1256
	_T("Check Forks"),											_T("����ϲ�"),									//1257
	_T("Value is negative, init carefully"),					_T("����ֵ,��С�ļ��,��Ҫ��ʼ�� "),			//1258
	_T("Logic Zero to Home"),									_T("ץƬ���߼����"),							//1259
	_T("Logic Right Limit to Home"),							_T("ץƬ���߼��Ҽ���"),							//1260
	_T("Standby Length (um)"),									_T("���ϵȴ�λ��(um)"),							//1261
	_T("Right Clearance (um)"),									_T("�Ҽ�����ֵ(um)"),							//1262
	_T("Front & Rear Rail"),									_T("ǰ�󵼹�"),									//1263
	_T("Please Manually Remove Window Clamper !"),				_T("���ֶ��Ƴ�֧�ܼоߣ�"),						//1264
	_T("Please Manually Install Window Clamper !"),				_T("���ֶ���װ֧�ܼоߣ�"),						//1265
	_T("Please Manually Remove Heater Block !"),				_T("���ֶ��Ƴ����ȼоߣ�"),						//1266
	_T("Please Manually Install Heater Block !"),				_T("���ֶ���װ���ȼоߣ�"),						//1267
	_T("Cannot find machine offset reference file !"),			_T("����ƫ���������ļ����ڣ�"),					//1268
	_T("Input Elevator Home fail !"),							_T("��������̨��ﲻ�ɻع���㣡"),				//1269
	_T("Input Side Clamper Home fail !"),						_T("���ϲ�о���ﲻ�ɻع���㣡"),				//1270    
	_T("Input Rear Clamper Home fail !"),						_T("���Ϻ�о���ﲻ�ɻع���㣡"),				//1271
	_T("Input Front Clamper Home fail !"),						_T("����ǰ�о���ﲻ�ɻع���㣡"),				//1272
	_T("Output Elevator Home fail !"),							_T("��������̨��ﲻ�ɻع���㣡"),				//1273
	_T("Output Side Clamper Home fail !"),						_T("���ϲ�о���ﲻ�ɻع���㣡"),				//1274    
	_T("Output Rear Clamper Home fail !"),						_T("���Ϻ�о���ﲻ�ɻع���㣡"),				//1275
	_T("Output Front Clamper Home fail !"),						_T("����ǰ�о���ﲻ�ɻع���㣡"),				//1276
	_T("Front Rail Home fail !"),								_T("ǰ������ﲻ�ɻع���㣡"),					//1277
	_T("Rear Rail Home fail !"),								_T("�󵼹���ﲻ�ɻع���㣡"),					//1278    
	_T("Indexer Home fail !"),									_T("ץƬ����ﲻ�ɻع���㣡"),					//1279
	_T("Window Clamper Home fail !"),							_T("֧�ܼо���ﲻ�ɻع���㣡"),				//1280
	_T("Wait Event Timeout!"),									_T("�ȴ��¼���ʱ!"),							//1281
	_T("Rail Touch with Heater Block"),							_T("��������������!"),							//1282
	_T("Heat Protect (mins)"),									_T("�����ȱ���ʱ��(����)"),						//1283
	_T("Load Material Handler Data?"),							_T("�Ƿ���ع���ϵͳ���ݣ�"),					//1284	//V1.1.W172 edit
	_T("FAB Type"),												_T("FAB ���"),									//1285
	_T("Is Bond Head at Safety Location ?"),					_T("���麸ͷ�ڰ�ȫλ��"),						//1286
	_T("1st Only"),												_T("ֻ��һ��Ԫ"),								//1287	//V1.4.W35 edit
	_T("AGC"),													_T("�Զ�����"),									//1288
	_T("Force,Auto"),											_T("ѹ��,�Զ�"),								//1289
	_T("Press 'Yes' to proceed, press 'No' to cancel"),			_T("��'ͬ��'��������'��ͬ��'ȡ��"),				//1290	//V1.1.W250 edit
	_T("Unit"),													_T("��Ԫ"),										//1291
	_T("Indexer fail !"),										_T("ץƬ������ƶ�ʧ�ܣ�"),						//1292
	_T("Edge to Index Hole:"),									_T("����������ؾ���"),							//1293
	_T("New Reel"),												_T("��֧�ܾ�"),									//1294 
	_T("Onload down!"),											_T("������ͣ��!"),								//1295
	_T("Offload down!"),										_T("������ͣ��!"),								//1296
	_T("Skip 1st "),											_T("������һ��Ԫ"),								//1297
	_T("Vacuum Check"),											_T("��ռ��"),									//1298
	_T("Index time > 16 "),										_T("������� > 16"),							//1299
	_T("Onload ERROR!"),										_T("����������!"),								//1300
	_T("Offload ERROR!"),										_T("����������!"),								//1301
	_T("Onloader Mag Pully Jam"),								_T("�������Ϻп�ס"),							//1302
	_T("Offloader Mag Pully Jam"),								_T("�������Ϻп�ס"),							//1303
	_T("Clamp Pos"),											_T("�г�λ��"),									//1304
	_T("Emergency Pressed!"),									_T("��ͣ���£�"),								//1305
	_T("No LF Pushed Out!"),									_T("�]֧���Ƴ���"),								//1306
	_T("Elevator Calibration"),									_T("������У׼"),								//1307
	_T("Next Mag Hold Pos:"),									_T("�Ϻмн�λ��:"),							//1308
	_T("Clamper Front Pos:"),									_T("�Ϻмо�ǰλ��:"),							//1309
	_T("Clamper Back Pos:"),									_T("�Ϻмоߺ�λ��:"),							//1310
	_T("Clamper Length Pos:"),									_T("�Ϻмо߳�λ��:"),							//1311
	_T("Front Track Pos:"),										_T("ǰ����λ��:"),								//1312
	_T("Back Track Pos:"),										_T("�󵼹�λ��:"),								//1313
	_T("Clamper Len Limit1:"),									_T("�Ϻмо߳���λ1:"),							//1314
	_T("Clamper Len Limit2:"),									_T("�Ϻмо߳���λ2:"),							//1315
	_T("Clamper Len Limit3:"),									_T("�Ϻмо߳���λ3:"),							//1316
	_T("Search Onloader MagLen Limit Fail!"),					_T("���������Ϻмо߳���λʧ�ܣ�"),				//1317
	_T("Search Onffloader MagLen Limit Fail!"),					_T("���������Ϻмо߳���λʧ�ܣ�"),				//1318
	_T("Center Offset:"),										_T("����ƫ��:"),								//1319
	_T("Whether Load Bond Parameter?"),							_T("�Ƿ���غ��߲�����"),						//1320
	_T("Whether Load Material Parameter?"),						_T("�Ƿ�������ϲ�����"),						//1321
	_T("Period of validity: "),									_T("��Ч��������"),								//1322
	_T("Please contact the supplier to unlock!"),				_T("����ϵ�豸��Ӧ�̽�����"),					//1323
	_T("Unlock Success!"),										_T("�����ɹ���"),								//1324
	_T("Unlock Fail!"),											_T("����ʧ�ܣ�"),								//1325
	_T("Unlock"),												_T("����"),										//1326
	_T("Period of validity: "),									_T("��Ч�ڣ�"),									//1327
	_T("Open Track"),											_T("�򿪵���"),									//1328
	_T("Close Track"),											_T("�رյ���"),									//1329
	_T("Unplug?"),												_T("�Ƿ�Ҫͨ�ɾף�"),							//1330
	_T("Onloader Total Slot"),									_T("�����Ϻ��ܲ���"),							//1331
	_T("Offloader Total Slot"),									_T("�����Ϻ��ܲ���"),							//1332
	_T("Auto Skip Score:"),										_T("�Զ�����������"),							//1333
	_T("Skipped the whole Unit, Contiune?"),					_T("������������Ԫ���Ƿ������"),				//1334
	_T("PR Fail!"),												_T("ͼ��ƥ��ʧ�ܣ�"),							//1335
	_T("PR Quality Reject/ Out of range!"),						_T("ͼ��ͼ��������/���ޣ�"),					//1336
	_T("PR Index Score"),										_T("����ͼ�����"),								//1337
	_T("Manual Skip"),											_T("�ֶ�����"),									//1338
	_T("Fast Loop"),											_T("���ٻ�������"),								//1339
	_T("Offload"),												_T("����"),										//1340
	_T("LF Kick Out Pos(X):"),									_T("��Ƭ��жƬλ��(X):"),						//1341
	_T("Mag Support Pos(Y&&Z):"),								_T("�Ϻ�֧��λ��(Y��Z):"),						//1342
	_T("Mag Drop Pos(Z):"),										_T("�Ϻ���ȡλ��(Z):"),							//1343
	_T("First LF Pos(Y&&Z):"),									_T("��һƬ֧��λ��(Y��Z):"),					//1344
	_T("Mag Offload Pos(Y&&Z):"),								_T("�Ϻ�ж��λ��(Y��Z):"),						//1345
	_T("Mag Pitch && Slot No.:"),								_T("�Ϻ��м�������:"),						//1346
	_T("LF Kicker"),											_T("֧����Ƭ��"),								//1347
	_T("Mag Indexer Y"),										_T("�Ϻ�ץƬ��Y"),								//1348
	_T("Mag Indexer Z"),										_T("�Ϻ�ץƬ��Z"),								//1349
	_T("Mag Kicker"),											_T("�Ϻ���Ƭ��"),								//1350
	_T("Mag Arm Empty"),										_T("�Ϻк��ƶ����ѿ�"),							//1351
	_T("Mag Drop Failed"),										_T("�����Ϻ�����ʧ��"),							//1352
	_T("Mag Kicker Full"),										_T("�����Ϻ���Ƭ������"),						//1353
	_T("Mag Arm Full"),											_T("�Ϻк��ƶ�������"),							//1354
	_T("Load Mag"),												_T("�����Ϻ�"),									//1355
	_T("Pitch +"),												_T("��ǰ�Ϻ�"),									//1356
	_T("Pitch -"),												_T("�ƺ��Ϻ�"),									//1357
	_T("LF Output Jam"),										_T("�Ϻ�֧�ܿ�ס!"),							//1358
	_T("Eject Mag"),											_T("�߳��Ϻ�"),									//1359
	_T("Bond Force Limit:"),									_T("����ѹ������"),								//1360
	_T("Pre Impact Force 1"),									_T("���Ԥ��ѹ��һ"),							//1361
	_T("Pre Impact Force 2"),									_T("���Ԥ��ѹ����"),							//1362
	_T("Pre Impact Time 1"),									_T("���Ԥ��ʱ��һ"),							//1363
	_T("Pre Impact Time 2"),									_T("���Ԥ��ʱ���"),							//1364
	_T("Select Wire:"),											_T("ѡ�����ߣ�"),								//1365
	_T("Backup PR 1"),											_T("����ģ��һ"),								//1366
	_T("Backup PR 2"),											_T("����ģ���"),								//1367
	_T("Backup PR 3"),											_T("����ģ����"),								//1368
	_T("Backup PR 4"),											_T("����ģ����"),								//1369
	_T("1st Bond Point"),										_T("��һ����"),									//1370
	_T("2nd Bond Point"),										_T("�ڶ�����"),									//1371
	_T("Remove the Top Plate and Window Clamper"),				_T("���Ƴ����Ȱ��֧�ܼо�"),					//1372
	_T("Have the Top Plate and Window Clamper been Removed?"),	_T("ȷ���Ѿ��Ƴ����Ȱ��֧�ܼоߣ�"),			//1373
	_T("Be careful on Capillary! Unplug?"),						_T("ע��ɾײ��֣� ͨ�ɾ�?"),					//1374
	_T("Search Range Retry:"),									_T("����������Χ��"),							//1375
	_T("Relay Safety Duration:"),								_T("�̵�����ȫ���ޣ�"),							//1376
	_T("Output Mag Slot Sequence:"),							_T("�����Ϻв�˳��:"),							//1377	//V1.4.W35 edit
	_T("Standard"),												_T("��׼˳��"),									//1378
	_T("Alternate"),											_T("����˳��"),									//1379
	_T("Invalid file name (exceed 31 characters)"),				_T("�ļ������ܳ���31���ַ�"),					//1380
	_T("Spark Unknown Error!"),									_T("���δ֪����"),							//1381
	_T("Spark Missing Result!"),								_T("�������ʧ��"),							//1382
	_T("Next Step Pusher In:"),									_T("��һ���ƽ���"),								//1383
	_T("Close"),												_T("�˳�"),										//1384
	_T("Step 3:"),												_T("������:"),									//1385
	_T("Are you sure to save and overwrite current device setting?"),	_T("ȷ��������Ĳ��ϲ�����"),			//1386
	_T("PreClamp Ht Close Delay (ms):"),						_T("�гָ߶ȹر���ʱ:"),						//1387
	_T("Are you sure ready to Pre Clamp Height?"),				_T("ȷ������Ԥ�ӽ�λ�ã�"),						//1388
	_T("Twist Forward Height Offset"),							_T("Twist Forward Height Offset"),				//1389
	_T("Auto Pad Centering:"),									_T("�Զ����̾��У�"),							//1390
	_T("Please Remove Elevator Support Bar"),					_T("���Ƴ�����̨֧��֧��"),						//1391
	_T("Gap Fill:"),											_T("ȱ����䣺"),								//1392
	_T("Binary:"),												_T("��ֵ����"),									//1393
	_T("Centering Type:"),										_T("�������ͣ�"),								//1394
	_T("Gravity"),												_T("����"),										//1395
	_T("Circle"),												_T("Բ��"),										//1396
	_T("Bond Head Time Offset"),								_T("��ͷƫ��ʱ��"),								//1397
	_T("Full Conversion"),										_T("ȫ��ת��"),									//1398
	_T("Partial Conversion"),									_T("����ת��"),									//1399
	_T("Blob+Edge"),											_T("�����+�߽�"),								//1400
	_T("Edge+Blob"),											_T("�߽�+�����"),								//1401
	_T("Blob"),													_T("�����"),									//1402
	_T("Edge"),													_T("�߽�"),										//1403
	_T("PR Overlap!"),											_T("ͼ��ƥ�佻��"),								//1404
	_T("Die Overlap"),											_T("оƬ����"),									//1405
	_T("Heat Delay Unit:"),										_T("������ʱ��λ��"),							//1406
	_T("msec"),													_T("����"),										//1407
	_T("sec"),													_T("��"),										//1408
	_T("Next LF Wait Pos (mm):"),								_T("����֧�ܵȴ�λ�� (mm)��"),					//1409
	_T("Rework"),												_T("����"),										//1410
	_T("Press 'Confirm' to exit, press 'Stop' to cancel"),		_T("��'ȷ��'�˳�����'ֹͣ'ȡ��"),				//1411
	_T("User Level"),											_T("�û�����"),									//1412	//V1.1.W38 add
	_T("Minimize Window"),										_T("��С������"),								//1413	//V1.1.W51 add
	_T("Press 'Confirm' to update, press 'Stop' to cancel"),	_T("��'ȷ��'���£���'ֹͣ'ȡ��"),				//1414	//V1.1.W141 add
	_T("USG Output Level"),										_T("�������ģʽ"),								//1415	//V1.1.W149 add
	_T("Elevator Side"),										_T("����̨"),									//1416	//V1.1.W149 add
	_T("Clamp Distance"),										_T("�н�����"),									//1417	//V1.1.W149 add
	_T("Drive Speed:"),											_T("�н��ٶ�:"),								//1418	//V1.1.W149 add
	_T("Vacuum Delay:"),										_T("�����ʱ:"),								//1419	//V1.1.W149 add
	_T("Skip Die:"),											_T("����оƬ:"),								//1420	//V1.1.W250 add
	_T("Skip Unit:"),											_T("������Ԫ:"),								//1421	//V1.1.W250 add
	_T("Statistics of 'Skip Die/Unit' will be cleared"),		_T("'����оƬ/��Ԫ'��ͳ�ƽ�������"),			//1422	//V1.1.W250 add
	_T("Wire Length equal to zero! Wire Number: %d"),			_T("�߳������㣡 �ߺ�: %d"),					//1423	//V1.1.W254 add
	_T("MC#"),													_T("����#"),									//1424	//V1.1.W258 add
	_T("Warning On Skip Num Per Index"),						_T("ÿ�ν�������������Ŀ"),						//1425	//V1.1.W262 add
	_T("'Skip Die/Unit Num Per Index' exceeds limit"),			_T("'ÿ�ν���������оƬ/��Ԫ����'��������"),	//1426	//V1.1.W262 add
	_T("'Serial Skip Die/Unit Num' exceeds limit"),				_T("'����������оƬ/��Ԫ����'��������"),		//1427	//V1.1.W265 add
	_T("Flatten LF before PR Index"),							_T("����ͼ��ǰѹƽ֧��"),						//1428	//V1.4.W35 add
	_T("LF Flattening Duration(ms)"),							_T("֧��ѹƽ����ʱ��(ms)"),						//1429	//V1.4.W35 add
	_T("Exceed Power 1 Range!"),								_T("��������һ��Χ��"),							//1430	//v1.5.T4 add
	_T("Exceed Power 2 Range!"),								_T("������������Χ��"),							//1431	//v1.5.T7 add
	_T("Window Clamper Not Closed Properly"),					_T("֧�ܼо�û�йؽ�"),							//1432	//V1.5.W11 add
	_T("Window Clamper Not Opened Properly"),					_T("֧�ܼо�û�д�"),							//1433	//V1.5.W11 add
	_T("Exceed Force 1 Range!"),								_T("����ѹ��һ��Χ��"),							//1434
	_T("Exceed Force 2 Range!"),								_T("����ѹ������Χ��"),							//1435
	_T("Exceed Time 1 Range!"),									_T("����ʱ��һ��Χ��"),							//1436
	_T("Exceed Time 2 Range!"),									_T("����ʱ�����Χ��"),							//1437
	_T("Total Cycle"),											_T("��������"),									//1438	//v1.5.T71 add
	_T("Bond Process Configure2"),								_T("���ӹ������ö�"),							//1439	//v1.6.T126 add
	_T("Slow search failed!"),									_T("������ʧ��!"),								//1440	//v1.6.P31 add
	_T("Reset Current Value to Default"),						_T("���õ�ǰֵ"),								//1441	//v1.6.P196 add
	_T("Load Config"),											_T("�����豸����"),								//1442	//v1.6.P238 add
	_T("Time 2"),												_T("ʱ���"),									//1443
	_T("Time 3"),												_T("ʱ����"),									//1444
	_T("Current 1"),											_T("����һ"),									//1445
	_T("Current 2"),											_T("������"),									//1446
	_T("Current 3"),											_T("������"),									//1447
	_T("Time 1"),												_T("ʱ��һ"),									//1448
	_T("Verify Recipe"),										_T("��֤��ʽ"),									//1449
	_T("1st bond contact low"),									_T("һ���Ӵ��߶ȹ���"),							//1450
	_T("1st bond contact high"),								_T("һ���Ӵ��߶ȹ���"),							//1451
	_T("2nd bond contact low"),									_T("�����Ӵ��߶ȹ���"),							//1452
	_T("2nd bond contact high"),								_T("�����Ӵ��߶ȹ���"),							//1453
	_T("Search Height 1"),										_T("�����߶�һ"),								//1454	//v3.0.T2 add
	_T("Search Height 2"),										_T("�����߶ȶ�"),								//1455	//v3.0.T2 add
	_T("Load Machine Config..."),								_T("�����豸����..."),							//1456	//v3.0.T137 add
	_T("Init Device Parameter..."),								_T("��ʼ�����ϲ���..."),						//1457	//v3.0.T137 add
	_T("Init Stepper..."),										_T("��ʼ���������..."),						//1458	//v3.0.T137 add
	_T("Init Servo..."),										_T("��ʼ���ŷ����..."),						//1459	//v3.0.T137 add
	_T("Init Machine Parameter..."),							_T("��ʼ���豸����..."),						//1460	//v3.0.T137 add
	_T("Check software exit normally or not..."),				_T("�������Ƿ������˳�..."),					//1461	//v3.0.T137 add
	_T("Init CAN Channel 1..."),								_T("��ʼ��CAN����ͨ��һ..."),					//1462	//v3.0.T137 add
	_T("Init USG..."),											_T("��ʼ��������..."),							//1463	//v3.0.T137 add
	_T("Initializing..."),										_T("��ʼ��..."),								//1464	//v3.0.T137 add
	_T("Init Lighting Comm..."),								_T("��ʼ���ƹ�ͨѶ..."),						//1465	//v3.0.T137 add
	_T("Init Vision System..."),								_T("��ʼ��ͼ��ϵͳ..."),						//1466	//v3.0.T137 add
	_T("Load Power-On Parameter..."),							_T("���ؿ�������..."),							//1467	//v3.0.T137 add
	_T("Init Bonder..."),										_T("��ʼ���豸..."),							//1468	//v3.0.T137 add
	_T("Init EFO..."),											_T("��ʼ�������..."),							//1469	//v3.0.T137 add
	_T("Init Wire Clamp..."),									_T("��ʼ�������..."),							//1470	//v3.0.T137 add
	_T("Init Material Handler..."),								_T("��ʼ������ϵͳ..."),						//1471	//v3.0.T137 add
	_T("Init XY Table..."),										_T("��ʼ��XY����̨..."),						//1472	//v3.0.T137 add
	_T("Load PR Pattern..."),									_T("����ͼ��ģ��..."),							//1473	//v3.0.T137 add
	_T("This Chip Is Reverse!"),								_T("��оƬ��λ�õߵ�"),							//1474	//v3.0.L284 add
	_T("Move table to left 2nd unit point"),					_T("�ƶ�����̨�������Ԫ��λ��"),				//1475	//v3.0.T311 add
	_T("Move table to down 2nd unit point"),					_T("�ƶ�����̨���¶���Ԫ��λ��"),				//1476	//v3.0.T311 add
	//v3.0.T433 add
	_T("BBOS Offset"),											_T("ѹ���߲���"),								//1477
	_T("BBOS Base Height (um)"),								_T("ѹ������߶� (um)"),						//1478
	_T("BBOS Span Length (um)"),								_T("ѹ��б�糤�� (um)"),						//1479
	_T("BBOS Ball Thickness (um)"),								_T("ѹ�����κ�� (um)"),						//1480
	_T("BBOS Scrub Distance (um)"),								_T("ѹ����о��� (um)"),						//1481
	_T("BBOS Cut Power (Dac)"),									_T("ѹ���������� (Dac)"),						//1482
	_T("BBOS Cut Power Delay (%)"),								_T("ѹ������������ʱ (%)"),						//1483
	_T("BBOS Motion Speed (%)"),								_T("ѹ���˶��ٶ� (%)"),							//1484
	_T("BBOS Direction Angle (Deg)"),							_T("ѹ����Ƕ� (��)"),						//1485
	//v3.0.T434 add
	_T("BBOS Pre Impact Time 1"),								_T("ѹ��Ԥ��ʱ��һ"),							//1486
	_T("BBOS Pre Impact Power 1"),								_T("ѹ��Ԥ������һ"),							//1487
	_T("BBOS Pre Impact Force 1"),								_T("ѹ��Ԥ��ѹ��һ"),							//1488
	_T("BBOS Impact Time 1"),									_T("ѹ����ʱ��һ"),							//1489
	_T("BBOS Impact Time 2"),									_T("ѹ����ʱ���"),							//1490
	_T("BBOS Impact Power 1"),									_T("ѹ��������һ"),							//1491
	_T("BBOS Impact Power 2"),									_T("ѹ����������"),							//1492
	_T("BBOS Impact Force 1"),									_T("ѹ����ѹ��һ"),							//1493
	_T("BBOS Impact Force 2"),									_T("ѹ����ѹ����"),							//1494
	_T("BBOS Time 1"),											_T("ѹ��ʱ��һ"),								//1495
	_T("BBOS Time 2"),											_T("ѹ��ʱ���"),								//1496
	_T("BBOS Power 1"),											_T("ѹ������һ"),								//1497
	_T("BBOS Power 2"),											_T("ѹ��������"),								//1498
	_T("BBOS Force 1"),											_T("ѹ��ѹ��һ"),								//1499
	_T("BBOS Force 2"),											_T("ѹ��ѹ����"),								//1500
	_T("Auto Search"),											_T("�Զ�����"),									//1501
	_T("Select 'Operation', then press 'Start'"),				_T("ѡ��'����', Ȼ��'��ʼ'"),					//1502
	_T("- Install indexer alignment jig"),						_T("- ��װץƬ��У׼����"),						//1503
	_T("- Press arrow keys until jig is aligned at"),			_T("- �����¼�ֱ��У׼����"),					//1504
	_T("  center mark of bond site"),							_T("  �ͺ������Ķ���"),							//1505
	_T("Press 'Up/Down' to adjust capillary level"),			_T("��'����'�����ɾ׸߶�"),						//1506	//v3.0.T454 add
	_T("Press 'Edit' to fine tune slot position"),				_T("��'�༭'΢���ۼ��"),						//1507
	_T("Above Loop"),											_T("����"),										//1508	//v3.1.T288 add
	_T("Fast Loop"),											_T("����"),										//1509	//v3.1.T288 add
	_T("BBOS Bump Shape Mode"),									_T("ѹ������ģʽ"),								//1510	//v3.1.T362 add
	_T("BBOS FAB Type"),										_T("ѹ�� FAB ���"),							//1511	//v3.1.T381 add
	_T("Bump Wire Parameter"),									_T("�����߲���"),								//1512	//v3.1.T401 add
	_T("BSOB Wire Pre Impact Time"),							_T("������Ԥ��ʱ��"),							//1513	//v3.1.T401 add
	_T("BSOB Wire Pre Impact Force"),							_T("������Ԥ��ѹ��"),							//1514	//v3.1.T401 add
	_T("BSOB Wire Pre Impact Power"),							_T("������Ԥ������"),							//1515	//v3.1.T401 add
	_T("BSOB Wire Impact Time"),								_T("�����߳��ʱ��"),							//1516	//v3.1.T401 add
	_T("BSOB Wire Impact Force"),								_T("�����߳��ѹ��"),							//1517	//v3.1.T401 add
	_T("BSOB Wire Impact Power"),								_T("�����߳������"),							//1518	//v3.1.T401 add
	_T("BSOB Wire Bond Time"),									_T("�����ߺ���ʱ��"),							//1519	//v3.1.T401 add
	_T("BSOB Wire Bond Force"),									_T("�����ߺ���ѹ��"),							//1520	//v3.1.T401 add
	_T("BSOB Wire Bond Power"),									_T("�����ߺ�������"),							//1521	//v3.1.T401 add
	_T("BSOB Wire Post Damp Time"),								_T("�����ߺ���ʱ��"),							//1522	//v3.1.T401 add
	_T("BSOB Wire Post Damp Force"),							_T("�����ߺ���ѹ��"),							//1523	//v3.1.T401 add
	_T("BSOB Wire Post Damp Power"),							_T("�����ߺ�������"),							//1524	//v3.1.T401 add
	_T("Position Offset"),										_T("λ��ƫ��"),									//1525	//v3.1.T410 add
	_T("Span Length Delay"),									_T("�����ʱ"),									//1526	//v3.1.T410 add
	_T("Arc Span"),												_T("�����"),									//1527	//v3.1.T410 add
	_T("Arc Span Delay"),										_T("�������ʱ"),								//1528	//v3.1.T410 add
	_T("Touchdown Force (gm)"),									_T("̽��ѹ�� (gm)"),							//1529	//v3.1.T432 add
	_T("Touchdown Range (%)"),									_T("̽��ѹ����Χ (%)"),							//1530	//v3.1.T432 add
	_T("Touchdown Time (ms)"),									_T("̽��ʱ�� (ms)"),							//1531	//v3.1.T432 add
	_T("Gap Large Time (10us)"),								_T("������ʱ��(10us)"),						//1532	//v3.1.T484 add

	_T("Message Out Range"),									_T("��Ϣ���"),
};

CMsgManager theMsg;

CMsgManager::CMsgManager(void)
{
}

CMsgManager::~CMsgManager(void)
{
}

void CMsgManager::Init()	//V1.2.W12 add
{
	m_bMsgLock = false;
	m_bProcessKeyboard = false;
}

void CMsgManager::GetMsg(short nMsgID, WCHAR *pcMsg)
{
	if (nMsgID > APP_MAX_MSG) nMsgID = APP_MAX_MSG;
	wcscpy_s(pcMsg, 80, wcMsgArray[nMsgID][theMcConfig.GetLanguage()]);
}

CString CMsgManager::GetMsg(short nMsgID)		//V1.1.W145 add
{
	if ((nMsgID < 0) || (nMsgID > APP_MAX_MSG)) nMsgID = APP_MAX_MSG;
	return wcMsgArray[nMsgID][theMcConfig.GetLanguage()];
}

//v2.0.T80 add
CString CMsgManager::GetEngMsg(short nMsgID)
{
	if ((nMsgID < 0) || (nMsgID > APP_MAX_MSG)) nMsgID = APP_MAX_MSG;
	return wcMsgArray[nMsgID][0];
}

// Show message box for help, error, or warning
int CMsgManager::ShowMsg(short nMsgType, short nMsgNum, short *pnMsgIndex, bool bStopValid, bool bSkipValid, bool bCenter)
{
    int iRet;

    CHelpMsgDlg* pMsgDlg = new CHelpMsgDlg(bStopValid, bSkipValid);
	pMsgDlg->CenterDialog(bCenter);

    switch (nMsgType)
    {
    case HELP_MSG:
        pMsgDlg->SetMessageBoxTitle(nMsgType, MSG_HELP_MSG);
        break;
    case ERROR_MSG:
        pMsgDlg->SetMessageBoxTitle(nMsgType, MSG_ERROR_MSG);
        break;
    case WARNING_MSG:
        pMsgDlg->SetMessageBoxTitle(nMsgType, MSG_WARNING_MSG);
        break;
    default:
        pMsgDlg->SetMessageBoxTitle(nMsgType, MSG_HELP_MSG);
        break;
    }

    pMsgDlg->SetMessageList(nMsgNum, pnMsgIndex);

	//if (m_pMsgDlg_Showed)
	//	m_pMsgDlg_Showed->KillTimer(1);
	
    if (bStopValid || bSkipValid)
    {
        pMsgDlg->DoModal();

        if (pMsgDlg->m_bStopSelected == true)
            iRet = SHOW_MSG_RESULT_STOP;
        else if (pMsgDlg->m_bSkipSelected == true)
            iRet = SHOW_MSG_RESULT_SKIP;
        else
            iRet = SHOW_MSG_RESULT_OK;
    }
    else
    {
        //pMsgDlg->Create(CHelpMsgDlg::IDD);
        pMsgDlg->DoModal();
        iRet = SHOW_MSG_RESULT_OK;
    }

	//if (m_pMsgDlg_Showed)
	//	m_pMsgDlg_Showed->SetTimer(1,1000,NULL);

    delete pMsgDlg;

    return iRet;
}

// Show message box for help, error, or warning
int CMsgManager::ShowMsg(short nMsgType, CStringList& sList, bool bStopValid, bool bSkipValid)
{
    int iRet;

    CHelpMsgDlg* pMsgDlg = new CHelpMsgDlg(bStopValid, bSkipValid);

    switch (nMsgType)
    {
    case HELP_MSG:
        pMsgDlg->SetMessageBoxTitle(nMsgType, MSG_HELP_MSG);
        break;
    case ERROR_MSG:
        pMsgDlg->SetMessageBoxTitle(nMsgType, MSG_ERROR_MSG);
        break;
    case WARNING_MSG:
        pMsgDlg->SetMessageBoxTitle(nMsgType, MSG_WARNING_MSG);
        break;
    default:
        pMsgDlg->SetMessageBoxTitle(nMsgType, MSG_HELP_MSG);
        break;
    }

    pMsgDlg->SetMessageList(sList);

	//if (m_pMsgDlg_Showed)
	//	m_pMsgDlg_Showed->KillTimer(1);

    if (bStopValid || bSkipValid)
    {
		pMsgDlg->DoModal();

        if (pMsgDlg->m_bStopSelected == true)
            iRet = SHOW_MSG_RESULT_STOP;
        else if (pMsgDlg->m_bSkipSelected == true)
            iRet = SHOW_MSG_RESULT_SKIP;
        else
            iRet = SHOW_MSG_RESULT_OK;
    }
    else
    {
        //pMsgDlg->Create(CHelpMsgDlg::IDD);
        pMsgDlg->DoModal();
        iRet = SHOW_MSG_RESULT_OK;
    }

	//if (m_pMsgDlg_Showed)
	//	m_pMsgDlg_Showed->SetTimer(1,1000,NULL);

    delete pMsgDlg;

    return iRet;
}

//for different words, how message box for help, error, or warning
int CMsgManager::ShowMsgX(short nMsgType, CStringList& sList, bool bStopValid, bool bSkipValid, short nMode)
{
    int iRet;

    CHelpMsgDlg* pMsgDlg = new CHelpMsgDlg(bStopValid,bSkipValid,0,0,0,0,nMode);

    switch (nMsgType)
    {
    case HELP_MSG:
        pMsgDlg->SetMessageBoxTitle(nMsgType, MSG_HELP_MSG);
        break;
    case ERROR_MSG:
        pMsgDlg->SetMessageBoxTitle(nMsgType, MSG_ERROR_MSG);
        break;
    case WARNING_MSG:
        pMsgDlg->SetMessageBoxTitle(nMsgType, MSG_WARNING_MSG);
        break;
    default:
        pMsgDlg->SetMessageBoxTitle(nMsgType, MSG_HELP_MSG);
        break;
    }

    pMsgDlg->SetMessageList(sList);
	pMsgDlg->DoModal();
	if (pMsgDlg->m_bStopSelected == true)
		iRet = SHOW_MSG_RESULT_STOP;	//second button
	else if (pMsgDlg->m_bSkipSelected == true)
		iRet = SHOW_MSG_RESULT_SKIP;	//third button
	else
		iRet = SHOW_MSG_RESULT_OK;		//first button

    delete pMsgDlg;
    return iRet;
}

// Show message box for help, error, or warning
int CMsgManager::ShowMhsMsg(short nMsgType, CStringList& sList, short nCallBackId, short nClearTrackId)
{
	if (IsMsgLock())
	{
		switch (nMsgType)
		{
		case HELP_MSG:
			m_MhsMsgDlg.SetMessageBoxTitle(nMsgType, MSG_HELP_MSG);
			break;
		case ERROR_MSG:
			m_MhsMsgDlg.SetMessageBoxTitle(nMsgType, MSG_ERROR_MSG);
			break;
		case WARNING_MSG:
			m_MhsMsgDlg.SetMessageBoxTitle(nMsgType, MSG_WARNING_MSG);
			break;
		default:
			m_MhsMsgDlg.SetMessageBoxTitle(nMsgType, MSG_HELP_MSG);
			break;
		}

		m_MhsMsgDlg.SetMessageList(sList);
		m_MhsMsgDlg.UpdateData(FALSE);
		bool bIsInvalidate = false;
		short nMsgNum = sList.GetSize();
		for (int i = 0; i < sList.GetCount(); i++)
		{
			if (m_asString[i] != sList.GetAt(sList.FindIndex(i)))
			{
				m_asString[i] = sList.GetAt(sList.FindIndex(i));
				bIsInvalidate = true;
			}
		}
		if (bIsInvalidate)
			m_MhsMsgDlg.Invalidate();
		//Ҫ�Ѵ����ö���ֻҪ��show������Dlg����Դ�����е�Topmost���ó�true����
		//::ShowWindow(pMsgDlg_Showed->m_hWnd,SW_RESTORE) ;
		//::BringWindowToTop(pMsgDlg_Showed->m_hWnd) ;
	}
	else
	{
		SetMsgLock(true);

		switch (nMsgType)
		{
		case HELP_MSG:
			m_MhsMsgDlg.SetMessageBoxTitle(nMsgType, MSG_HELP_MSG);
			break;
		case ERROR_MSG:
			m_MhsMsgDlg.SetMessageBoxTitle(nMsgType, MSG_ERROR_MSG);
			break;
		case WARNING_MSG:
			m_MhsMsgDlg.SetMessageBoxTitle(nMsgType, MSG_WARNING_MSG);
			break;
		default:
			m_MhsMsgDlg.SetMessageBoxTitle(nMsgType, MSG_HELP_MSG);
			break;
		}

		m_MhsMsgDlg.SetMessageList(sList);
		m_MhsMsgDlg.m_nClearTrack = nClearTrackId;
		m_MhsMsgDlg.m_nCallBackId = nCallBackId;
		m_MhsMsgDlg.Create(CWarnMsgDlg::IDD);
		m_MhsMsgDlg.ShowWindow(TRUE);
	}
    return SHOW_MSG_RESULT_OK;
}

void CMsgManager::SetMsgLock(bool bLock)
{
    m_bMsgLock = bLock;
}

bool CMsgManager::IsMsgLock(void)
{
    return m_bMsgLock;
}

int CMsgManager::ShowMsgS(short nMsgType, CStringList& sList, bool bStopValid, bool bSkipValid,bool bCtrLight)
{
    CHelpMsgDlg MsgDlg(bStopValid, bSkipValid, 0, true, NULL, bCtrLight);

    switch (nMsgType)
    {
    case HELP_MSG:
        MsgDlg.SetMessageBoxTitle(nMsgType, MSG_HELP_MSG);
        break;
    case ERROR_MSG:
        MsgDlg.SetMessageBoxTitle(nMsgType, MSG_ERROR_MSG);
        break;
    case WARNING_MSG:
        MsgDlg.SetMessageBoxTitle(nMsgType, MSG_WARNING_MSG);
        break;
    default:
        MsgDlg.SetMessageBoxTitle(nMsgType, MSG_HELP_MSG);
        break;
    }

    MsgDlg.SetMessageList(sList);

    if (bStopValid || bSkipValid)
    {
		if (nMsgType == HELP_MSG)
		{
			MsgDlg.Create(CHelpMsgDlg::IDD);
			MsgDlg.ShowWindow(SW_SHOW);
			if (m_bProcessKeyboard)
				((CWireBondDlg*)(MsgDlg.GetParent()))->SetProcessKeyboard(false);

			while (MsgDlg.IsWindowVisible())
			{
				MSG Msg;
				while(::PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
				{
					if ((Msg.message == WM_KEYDOWN) || (Msg.message == WM_KEYUP))
					{
						if ((theMcConfig.GetKeyPadType() >= 1)
							&& (theMcConfig.GetKeyPadType() <= 3))	//v2.0.T35 edit
						{
							if (theKeyPad.HotKeyHandler(&Msg) == TRUE)
								continue;
						}
					}
					CWnd *pWnd;
					CRect rect1,rect2;
					if (((CWireBondDlg*)(MsgDlg.GetParent()))->m_pLightCtrlDlg->IsWindowVisible())
					{
						pWnd = ((CWireBondDlg*)(MsgDlg.GetParent()))->GetDlgItem(IDC_IMG);
						pWnd->GetWindowRect(&rect1);
						MsgDlg.GetWindowRect(&rect2);
						if (Msg.pt.x > 0 && Msg.pt.x < rect1.left
							&& Msg.pt.y > 0 && Msg.pt.y < rect2.top)
						{
							continue;
						}
					}
					::TranslateMessage(&Msg);
					::DispatchMessage(&Msg);
				}
				Sleep(3);
			}
		}
		else
			MsgDlg.DoModal();

		((CWireBondDlg*)(MsgDlg.GetParent()))->SetProcessKeyboard(m_bProcessKeyboard);
        if (MsgDlg.m_bStopSelected == true)
            return SHOW_MSG_RESULT_STOP;
        else if (MsgDlg.m_bSkipSelected == true)
            return SHOW_MSG_RESULT_SKIP;
        else
            return SHOW_MSG_RESULT_OK;
    }
    else
    {
        //MsgDlg.Create(CHelpMsgDlg::IDD);
		if (nMsgType == HELP_MSG)
		{
			MsgDlg.Create(CHelpMsgDlg::IDD);
			MsgDlg.ShowWindow(SW_SHOW);
			((CWireBondDlg*)(MsgDlg.GetParent()))->InhibitSystemButton();
			((CWireBondDlg*)(MsgDlg.GetParent()))->SetProcessKeyboard(false);
			while (MsgDlg.IsWindowVisible())
			{
				MSG Msg;
				while(::PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
				{
					if ((Msg.message == WM_KEYDOWN) || (Msg.message == WM_KEYUP))
					{
						if ((theMcConfig.GetKeyPadType() >= 1)
							&& (theMcConfig.GetKeyPadType() <= 3))	//v2.0.T35 edit
						{
							if (theKeyPad.HotKeyHandler(&Msg) == TRUE)
								continue;
						}
					}

					CWnd *pWnd;
					CRect rect,rect1,rect2;
					if (((CWireBondDlg*)(MsgDlg.GetParent()))->m_pLightCtrlDlg->IsWindowVisible())
					{
						pWnd = ((CWireBondDlg*)(MsgDlg.GetParent()))->GetDlgItem(IDC_IMG);
						pWnd->GetWindowRect(&rect1);
						MsgDlg.GetWindowRect(&rect2);
						if ((Msg.pt.x > 0) && (Msg.pt.x > rect1.left)
							&& (Msg.pt.y > 0) && (Msg.pt.y < rect2.top))
						{
							continue;
						}
					}
					else
					{
						pWnd = ((CWireBondDlg*)(MsgDlg.GetParent()))->GetDlgItem(IDC_IMG);
						pWnd->GetWindowRect(&rect1);
						MsgDlg.GetWindowRect(&rect2);
						if ((Msg.pt.x < rect1.left) || (Msg.pt.x > rect1.right)
							|| (Msg.pt.y < rect1.top) || (Msg.pt.y > rect2.bottom))
						{
							continue;
						}
					}
					
					::TranslateMessage(&Msg);
					::DispatchMessage(&Msg);
				}
				Sleep(3);
			}
		}
		else
			MsgDlg.DoModal();

		((CWireBondDlg*)(MsgDlg.GetParent()))->EnableSystemButton();
		((CWireBondDlg*)(MsgDlg.GetParent()))->SetProcessKeyboard(true);

        return SHOW_MSG_RESULT_OK;
    }
}

int CMsgManager::ShowDynamicBtnMsg(CStringList& sList, short nOkBtnTextID, short nCancelBtnTextID)
{
    CDyBtnMsgDlg* pMsgDlg = new CDyBtnMsgDlg(nOkBtnTextID, nCancelBtnTextID);
    pMsgDlg->SetMessageList(sList);
    pMsgDlg->DoModal();

    if (pMsgDlg->m_bOkSelected)
        return SHOW_MSG_RESULT_OK;
    else if (pMsgDlg->m_bCancelSelected)
        return SHOW_MSG_RESULT_STOP;
	else
		return SHOW_MSG_RESULT_STOP;
}

BYTE CMsgManager::IsEqual(short nMsg, LPTSTR psMsg)	//V1.1.W154 add
{
	if (psMsg == NULL) return FALSE;
	if ((nMsg < 0) || (nMsg > APP_MAX_MSG)) nMsg = APP_MAX_MSG;
	return (lstrcmp(wcMsgArray[nMsg][theMcConfig.GetLanguage()], psMsg) == 0);
}

//v2.0.T33 add
void CMsgManager::SetProcessKeyboard(bool bProcess)
{
	m_bProcessKeyboard = bProcess;
}
