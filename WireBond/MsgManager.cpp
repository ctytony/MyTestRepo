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
	_T("中文"),													_T("English"),									//0
	_T("Setup"),												_T("校准"),										//1
	_T("Recipe"),												_T("程式"),										//2
	_T("Run"),													_T("运行"),										//3
	_T("Config"),												_T("配置"),										//4
	_T("Vision Calibration"),									_T("图像校准"),									//5
	_T("Shut Down"),											_T("退出"),										//6		//V1.1.W71 edit
	_T("Teach Recipe"),											_T("学习程式"),									//7
	_T("Single Bond"),											_T("单一焊线"),									//8
	_T("Auto Bond"),											_T("连续焊线"),									//9
	_T("System Configure"),										_T("系统设定"),									//10
	_T("Utility"),												_T("应用"),										//11
	_T("Debug"),												_T("系统调试"),									//12
	_T("Dummy Recipe"),											_T("虚拟程式"),									//13
	_T("Press Start to teach dummy recipe area"),				_T("按'开始'键指定虚拟程式区域"),				//14
	_T("Move table to top-left position"),						_T("移动工作台到左上角"),						//15
	_T("Move table to bottom-right position"),					_T("移动工作台到右下角"),						//16
	_T("Press 'Next' to continue"),								_T("按'下一步'继续"),							//17	//V1.1.W139 edit
	_T("Teach dummy program completed"),						_T("虚拟程式学习完成"),							//18
	_T("Recipe Manage"),										_T("程式管理"),									//19
	_T("Load Recipe"),											_T("加载程式"),									//20
	_T("Save Recipe"),											_T("保存程式"),									//21
	_T("Clear Recipe"),											_T("清除程式"),									//22
	_T("Help Message"),											_T("帮助信息"),									//23
	_T("Error Message"),										_T("错误信息"),									//24
	_T("Warning Message"),										_T("警告信息"),									//25
	_T("Bond Parameter"),										_T("焊接参数"),									//26
	_T("Loop Parameter"),										_T("线弧参数"),									//27
	_T("Ball"),													_T("球焊"),										//28
	_T("Wedge"),												_T("线焊"),										//29
	_T("Impact Time (ms)"),										_T("前置时间 (ms)"),							//30	//V1.1.W149 edit
	_T("Impact Power (Dac)"),									_T("前置能量 (Dac)"),							//31	//V1.1.W149 edit
	_T("Impact Force (gm)"),									_T("前置压力 (gm)"),							//32	//V1.1.W149 edit
	_T("Bond Time (ms)"),										_T("焊接时间 (ms)"),							//33
	_T("Bond Power (Dac)"),										_T("焊接能量 (Dac)"),							//34
	_T("Bond Force (gm)"),										_T("焊接压力 (gm)"),							//35
	_T("Post Damp Time (ms)"),									_T("后置时间 (ms)"),							//36	//V1.1.W149 edit
	_T("Post Damp Power (Dac)"),								_T("后置能量 (Dac)"),							//37	//V1.1.W149 edit
	_T("Post Damp Force (gm)"),									_T("后置压力 (gm)"),							//38	//V1.1.W149 edit
	_T("Power Equalization (Dac)"),								_T("能量均化系数 (Dac)"),						//39
	_T("Force Equalization (gm)"),								_T("压力均化系数 (gm)"),						//40
	_T("Done"),													_T("完成"),										//41
	_T("Wire Offset"),											_T("单线参数"),									//42	//v3.0.T429 edit
	_T("Loop Group"),											_T("线弧组别"),									//43
	_T("Loop Type"),											_T("线弧类型"),									//44
	_T("Loop Height (um)"),										_T("线弧高度 (um)"),							//45
	_T("Neck Distance (%)"),									_T("线颈长度 (%)"),								//46
	_T("Neck Angle (Deg)"),										_T("线颈角度 (度)"),							//47
	_T("Wire Length Correction (um)"),							_T("线长修正 (um)"),							//48
	_T("Kink2 Span Length"),									_T("拐点2跨度"),								//49	//v3.1.T320 edit
	_T("Kink2 Height Factor (%)"),								_T("拐点2高度比例 (%)"),						//50	//v3.1.T320 edit
	_T("Pull Distance (um)"),									_T("拉伸长度 (um)"),							//51
	_T("Neck Arc Factor (%)"),									_T("线颈弧度 (%)"),								//52	//v3.1.T407 add
	_T("Loop Base Mode"),										_T("弧底模式"),									//53
	_T("Kink2 Span Length Unit"),								_T("拐点2跨度单位"),							//54	//v3.1.T320 edit
	_T("Pre Impact Power (Dac)"),								_T("前置预备能量 (Dac)"),						//55
	_T("Neck Arc Delay (%)"),									_T("线颈弧度延时 (%)"),							//56	//v3.1.T407 add
	_T("Bond Setup"),											_T("焊接设置"),									//57
	_T("Reference"),											_T("参照点"),									//58
	_T("Type"),													_T("类型"),										//59
	_T("Contact Level"),										_T("接触高度"),									//60
	_T("Lead"),													_T("管脚"),										//61
	_T("Die"),													_T("芯片"),										//62
	_T("Gnd"),													_T("地线"),										//63
	_T("Move table to contact position"),						_T("移动工作台到接触位置"),						//64
	_T("Contact search successful"),							_T("接触面搜索成功"),							//65
	_T("Contact search failure"),								_T("接触面搜索失败"),							//66
	_T("New contact level is "),								_T("新接触高度是 "),							//67
	_T("Current (XY)"),											_T("当前值(XY)"),								//68
	_T("New (XY)"),												_T("更新值(XY)"),								//69
	_T("Move cursor to capillary mark"),						_T("移动光标到瓷咀印上"),						//70
	_T("Bond tip offset updated!"),								_T("瓷咀偏移量"),								//71
	_T("Lighting Parameters"),									_T("光线设置"),									//72
	_T("Binary"),												_T("黑白图像"),									//73
	_T("Grey"),													_T("灰度图像"),									//74
	_T("Cancel"),												_T("取消"),										//75
	_T("Press Start to teach alignment point"),					_T("按'开始'键指定定位点"),						//76
	_T("Move table to lead 1st alignment point"),				_T("移动工作台到管脚第一定位点"),				//77
	_T("Move table to lead 2nd alignment point"),				_T("移动工作台到管脚第二定位点"),				//78
	_T("Move table to die 1st alignment point"),				_T("移动工作台到芯片第一定位点"),				//79
	_T("Move table to die 2nd alignment point"),				_T("移动工作台到芯片第二定位点"),				//80
	_T("Adjust lighting for clear image"),						_T("请调整光线以显示清晰的图像"),				//81
	_T("Teaching alignment point:"),							_T("指定定位点:"),								//82
	_T("Teaching die alignment point"),							_T("指定芯片定位点"),							//83
	_T("If ready, press Continue"),								_T("准备好了，请按'继续'"),						//84
	_T("Press Next to teach vision pattern"),					_T("按'下一步'指定图像模板"),					//85
	_T("Teach alignment point finished"),						_T("指定定位点完成"),							//86
	_T("Press Start to teach vision pattern"),					_T("按'开始'键指定图像模板"),					//87
	_T("Teaching vision pattern:"),								_T("指定图像模板:"),							//88
	_T("Teaching die vision pattern"),							_T("指定芯片图像模板"),							//89
	_T("Move table to lead 1st pattern point"),					_T("移动工作台到管脚第一模板点"),				//90
	_T("Move table to lead 2nd pattern point"),					_T("移动工作台到管脚第二模板点"),				//91
	_T("Move table to die 1st pattern point"),					_T("移动工作台到芯片第一模板点"),				//92
	_T("Move table to die 2nd pattern point"),					_T("移动工作台到芯片第二模板点"),				//93
	_T("Teach vision pattern finished"),						_T("指定图像模板完成"),							//94
	_T("Press Next to teach wire layout"),						_T("按'下一步'指定连线布局"),					//95
	_T("Move table to wire 1st bond point"),					_T("移动工作台到连线第一焊点"),					//96
	_T("Move table to wire 2nd bond point"),					_T("移动工作台到连线第二焊点"),					//97
	_T("Neck Height Factor 2 (%)"),								_T("线颈高度比例二 (%)"),						//98	//v3.1.T407 add
	_T("Pre Kink Height Factor 2 (%)"),							_T("预折点高度比例二 (%)"),						//99	//v3.1.T407 add
	_T("Pre Kink Arc Factor (%)"),								_T("预折点弧度 (%)"),							//100	//v3.1.T407 add
	_T("Pre Kink Arc Delay (%)"),								_T("预折点弧度延时 (%)"),						//101	//v3.1.T407 add
	_T("Kink2 Height Factor 2 (%)"),							_T("拐点2高度比例二 (%)"),						//102
	_T("Kink2 Arc Factor (%)"),									_T("拐点2弧度 (%)"),							//103
	_T("Kink2 Arc Delay (%)"),									_T("拐点2弧度延时 (%)"),						//104
	_T("Kink3 Height Factor 2 (%)"),							_T("拐点3高度比例二 (%)"),						//105	//v3.1.T407 add
	_T("Press Start to teach sequence"),						_T("按'开始'键指定焊接顺序"),					//106	//v1.5.T91 edit
	_T("Move table to 1st unit alignment point"),				_T("移动工作台到第一单元定位点"),				//107
	_T("Kink3 Arc Factor (%)"),									_T("拐点3弧度 (%)"),							//108	//v3.1.T407 add
	_T("Kink3 Arc Delay (%)"),									_T("拐点3高度比例二 (%)"),						//109	//v3.1.T407 add
	_T("Move table to top-left unit alignment point"),			_T("移动工作台到左上角单元定位点"),				//110
	_T("Move table to last unit alignment point"),				_T("移动工作台到最后单元定位点"),				//111
	_T("Teach sequence finished"),								_T("指定焊接顺序完成"),							//112	//v1.5.T91 edit
	_T("Kink2 Height (%)"),										_T("拐点2高度 (%)"),							//113	//v3.1.T407 add
	_T("Please select Bond Sequence"),							_T("请选择焊接顺序"),							//114
	_T("Press Calibrate to learn vision factor"),				_T("按'校准'键学习图像分辨率系数"),				//115
	_T("Move template to unique pattern point,"),				_T("移动工作台到一个模式独特的点"),				//116
	_T("Camara rotate angle too large!"),						_T("摄像头旋转角度太大!"),						//117
	_T("Vision search offset is zero!"),						_T("图像识别距离为0!"),							//118
	_T("Vision factor calibration fail!"),						_T("图像分辨率系数校准失败!"),					//119
	_T("Edit Recipe"),											_T("编辑程式"),									//120
	_T("Press Start to edit alignment point"),					_T("按'开始'键编辑定位点"),						//121
	_T("Edit Alignment Point"),									_T("编辑定位点"),								//122
	_T("Editing alignment point:"),								_T("编辑定位点:"),								//123
	_T("Kink3 Height (%)"),										_T("拐点3高度 (%)"),							//124
	_T("Edit alignment point finished"),						_T("编辑定位点完成"),							//125
	_T("Kink2 Span Length 2"),									_T("拐点2跨度二"),								//126
	_T("Press Next to edit vision pattern"),					_T("按'下一步'编辑图像模板"),					//127
	_T("Editing vision pattern:"),								_T("编辑图像模板:"),							//128
	_T("Editing die vision pattern"),							_T("编辑芯片图像模板"),							//129
	_T("Edit vision pattern finished"),							_T("编辑图像模板完成"),							//130
	_T("Press Next to edit wire layout"),						_T("按'下一步'编辑连线布局"),					//131
	_T("Press Start to teach fire level"),						_T("按'开始'指定烧球高度"),						//132
	_T("Press 'Up/Down' to adjust fire level"),					_T("按'上下'调整烧球高度"),						//133	//v3.0.T454 edit
	_T("Or key in fire level"),									_T("或输入烧球高度"),							//134
	_T("Press Confirm to update fire level"),					_T("按'确认'更新烧球高度"),						//135
	_T("Please do manual alignment"),							_T("请做手动定位"),								//136
	_T("Move table to 1st alignment point"),					_T("移动工作台到第一定位点"),					//137
	_T("Move table to 2nd alignment point"),					_T("移动工作台到第二定位点"),					//138
	_T("Servo System Configure"),								_T("伺服系统配置"),								//139
	_T("Material Handler Parameter"),							_T("过料系统参数"),								//140
	_T("Servo System Calibration"),								_T("伺服系统校准"),								//141
	_T("Kink3 Span Length 2"),									_T("拐点3跨度二"),								//142
	_T("Neck Span Length 2 (um)"),								_T("线颈跨度二 (um)"),							//143
	_T("Pre Kink Span Length 2 (um)"),							_T("预折点跨度二 (um)"),						//144
	_T("Force verification success"),							_T("压力验证成功"),								//145
	_T("Force verification fail"),								_T("压力验证失败"),								//146
	_T("Move table to dummy bond position"),					_T("移动工作台到焊点位置"),						//147
	_T("Ensure clear image and proper lighting"),				_T("确保图像清晰和光源足够"),					//148
	_T("Move table to teach track offset"),						_T("移动工作台学习轨道偏移"),					//149
	_T("Move cursor to die center"),							_T("移动光标到芯片中心"),						//150
	_T("Press Next to verify bond tip offset"),					_T("按'下一步'验证瓷咀偏移"),					//151
	_T("Adjust optics to an unique pattern"),					_T("移动镜头到一独特模板"),						//152
	_T("Force (gm)"),											_T("压力 (gm)"),								//153
	_T("Wire Clamp"),											_T("线夹"),										//154
	_T("Contact Search"),										_T("接触面搜索"),								//155
	_T("Bond Tip Offset"),										_T("瓷咀偏移"),									//156
	_T("Fire Level"),											_T("烧球高度"),									//157
	_T("Bond"),													_T("焊接"),										//158
	_T("Dummy Bond"),											_T("做尾"),										//159
	_T("General"),												_T("常规"),										//160
	_T("Statistics"),											_T("统计"),										//161
	_T("Acceleration"),											_T("加速度"),									//162
	_T("Window Clamper"),										_T("支架夹具"),									//163
	_T("Input "),												_T("上料"),										//164
	_T("Track"),												_T("导轨"),										//165
	_T("Output "),												_T("下料"),										//166
	_T("Next"),													_T("下一步"),									//167
	_T("Prev"),													_T("上一步"),									//168
	_T("Stop Bond"),											_T("停机"),										//169
	_T("BTO"),													_T("修正瓷咀"),									//170	//V1.1.W149 edit
	_T("Correct Bond"),											_T("单一补线"),									//171	//V1.1.W149 edit
	_T("Ball Detect"),											_T("烧球检测"),									//172
	_T("Auto Index"),											_T("自动进料"),									//173	//V1.1.W149 edit
	_T("Dry Run"),												_T("空跑"),										//174
	_T("No Vision"),											_T("无定位"),									//175
	_T("Complete"),												_T("自动停机"),									//176
	_T("Bond Point"),											_T("焊点位置"),									//177	//V1.1.W149 edit
	_T("Current Wire #"),										_T("当前线号 #"),								//178
	_T("Time Parameters"),										_T("时间参数"),									//179
	_T("Stopper Motion:"),										_T("制动器动作:"),								//180
	_T("Picker Motion:"),										_T("机械手动作:"),								//181
	_T("Picker Delay:"),										_T("机械手延时:"),								//182
	_T("Stopper"),												_T("制动器"),									//183
	_T("Picker"),												_T("机械手"),									//184
	_T("Forward"),												_T("前向运动"),									//185
	_T("Backward"),												_T("后向运动"),									//186
	_T("On"),													_T("开"),										//187
	_T("Off"),													_T("关"),										//188
	_T("Inner"),												_T("内侧导轨"),									//189
	_T("Outer"),												_T("外侧导轨"),									//190
	_T("Clamper"),												_T("夹具"),										//191
	_T("Track:"),												_T("导轨:"),									//192
	_T("Parameters"),											_T("参数"),										//193
	_T("Fine Move Speed:"),										_T("精确运动速度:"),							//194
	_T("Rough Move Speed:"),									_T("粗略运动速度:"),							//195
	_T("Clamp Position:"),										_T("夹紧位置:"),								//196
	_T("Open Position:"),										_T("松开位置:"),								//197
	_T("Home"),													_T("回归起点"),									//198
	_T("Close"),												_T("夹紧"),										//199
	_T("Open"),													_T("松开"),										//200
	_T("Vision System Configure"),								_T("视觉系统配置"),								//201
	_T("Motion Delay"),											_T("运动延时"),									//202
	_T("Short Distance (ms):"),									_T("短距离(ms):"),								//203
	_T("Long Distance (ms):"),									_T("长距离(ms):"),								//204
	_T("Search Mode"),											_T("识别方式"),									//205
	_T("Single Lead PR:"),										_T("管脚单点识别:"),							//206
	_T("Pre Move:"),											_T("提前移动:"),								//207
	_T("Disable"),												_T("断电"),										//208
	_T("Enable"),												_T("上电"),										//209
	_T("Action"),												_T("动作"),										//210
	_T("Module:"),												_T("部件:"),									//211
	_T("PreTrack"),												_T("焊前导轨"),									//212
	_T("PostTrack"),											_T("焊后导轨"),									//213
	_T("Onload Track"),											_T("上料导轨"),									//214
	_T("Debug:"),												_T("调试工具"),									//215
	_T("216"),													_T("216"),										//216
	_T("Index"),												_T("料盒索引"),									//217
	_T("Elevator"),												_T("升降台"),									//218
	_T("Arm Standby"),											_T("机械手预备位"),								//219
	_T("Arm Sensor Distance:"),									_T("机械手感应器距离:"),						//220
	_T("Magazine Thickness:"),									_T("料盒厚度:"),								//221
	_T("Magazine Buffer Size:"),								_T("料盒缓存数:"),								//222
	_T("First Empty Mag"),										_T("首个空载料盒位"),							//223
	_T("First Full Mag"),										_T("首个满载料盒位"),							//224
	_T("Empty Mag Position"),									_T("空载料盒位"),								//225
	_T("Full Mag Position"),									_T("满载料盒位"),								//226
	_T("Magazine Buffer"),										_T("料盒缓存"),									//227
	_T("Magazine Storey:"),										_T("料盒层数:"),								//228
	_T("Empty Mag"),											_T("空载料盒"),									//229
	_T("Full Mag"),												_T("满载料盒"),									//230
	_T("Learn Pos"),											_T("自学位置"),									//231
	_T("First Slot"),											_T("料盒首槽"),									//232
	_T("Total Slots:"),											_T("料盒总槽数:"),								//233
	_T("Slot Pitch:"),											_T("支架槽间距:"),								//234
	_T("Track Space:"),											_T("内外导轨间距:"),							//235
	_T("Position Adjust"),										_T("位置调整"),									//236
	_T("Adjust Item:"),											_T("调整项目:"),								//237
	_T("Magazine Position:"),									_T("料盒位置:"),								//238
	_T("Adjust Step"),											_T("调整步距"),									//239
	_T("Move"),													_T("移动"),										//240
	_T("More"),													_T("多一些"),									//241
	_T("Less"),													_T("少一些"),									//242
	_T("Current Slot"),											_T("当前支架槽"),								//243
	_T("Slot Verify"),											_T("支架槽验证"),								//244
	_T("Speed (mm/s)"),											_T("速度 (mm/s)"),								//245
	_T("Clamp Stepper"),										_T("夹具步进电机"),								//246
	_T("Clamper:"),												_T("夹具:"),									//247
	_T("Index Stepper"),										_T("索引步进电机"),								//248
	_T("Elevator Stepper"),										_T("升降台步进电机"),							//249
	_T("Index Speed:"),											_T("料盒索引速度:"),							//250
	_T("Elevator Speed:"),										_T("升降台速度:"),								//251
	_T("Start"),												_T("开始"),										//252
	_T("Stop"),													_T("停止"),										//253
	_T("Low"),													_T("低"),										//254
	_T("Middle"),												_T("中"),										//255
	_T("High"),													_T("高"),										//256
	_T("Clear"),												_T("清除"),										//257
	_T("Up"),													_T("上移"),										//258
	_T("Down"),													_T("下移"),										//259
	_T("Arm Cylinder:"),										_T("机械手气缸"),								//260
	_T("Rotary Cylinder:"),										_T("旋转气缸"),									//261
	_T("Both"),													_T("双导轨"),									//262
	_T("Initialize"),											_T("初始化"),									//263
	_T("Recover"),												_T("恢复"),										//264
	_T("Calibrate"),											_T("校准"),										//265
	_T("Verify"),												_T("检验"),										//266
	_T("Finish"),												_T("完成"),										//267
	_T("Factors:"),												_T("系数:"),									//268
	_T("Apply Force:"),											_T("施加力:"),									//269
	_T("Measured Force:"),										_T("测量力:"),									//270
	_T("Measure wire clamp force with force gauge "),			_T("用测力计测量线夹夹紧力"),					//271
	_T("Key in measured force"),								_T("键入测量力"),								//272
	_T("Step 1:"),												_T("第一步:"),									//273
	_T("Step 2:"),												_T("第二步:"),									//274
	_T("Calibration finished"),									_T("校准完成"),									//275
	_T("Verification finished"),								_T("检验完成"),									//276
	_T("Current:"),												_T("当前值"),									//277
	_T("New:"),													_T("更新值"),									//278
	_T("Operation"),											_T("操作"),										//279
	_T("Fast"),													_T("快"),										//280
	_T("Very High"),											_T("很快"),										//281
	_T("Motion Type:"),											_T("运动类型:"),								//282
	_T("Speed:"),												_T("速度:"),									//283
	_T("Empty Motion"),											_T("空载运动"),									//284
	_T("Load Motion"),											_T("满载运动"),									//285
	_T("Index Motion"),											_T("料盒索引运动"),								//286
	_T("Search Motion"),										_T("料盒搜索运动"),								//287
	_T("Measure bond head force with force gauge"),				_T("用测力计测量焊头压紧力"),					//288
	_T("Advanced Bond"),										_T("高级焊接"),									//289
	_T("Search Height (um)"),									_T("搜索高度 (um)"),							//290
	_T("Search Threshold (um)"),								_T("搜索阈值 (um)"),							//291
	_T("Search Speed (mm/s)"),									_T("搜索速度 (mm/s)"),							//292
	_T("Spark"),												_T("电子打火"),									//293
	_T("Tail Length:"),											_T("线尾长度:"),								//294
	_T("Spark Delay"),											_T("打火延时"),									//295
	_T("Table position out of limit!"),							_T("工作台超出工作范围!"),						//296
	_T("Table Jogging Speed:"),									_T("工作台移动速度:"),							//297
	_T("Increase"),												_T("加快"),										//298
	_T("Decrease"),												_T("减慢"),										//299
	_T("LF Index"),												_T("进料"),										//300	//V1.1.W149 edit
	_T("Unit Information"),										_T("单元信息"),									//301
	_T("Current Unit:"),										_T("当前单元:"),								//302
	_T("Row"),													_T("行"),										//303
	_T("Column"),												_T("列"),										//304
	_T("Unit Layout"),											_T("单元布局"),									//305
	_T("Bond Shift (um)"),										_T("焊头滑移 (um)"),							//306
	_T("Apply"),												_T("应用"),										//307
	_T("Wcl Tail Close (ms/2)"),								_T("线夹线尾高度关闭 (ms/2)"),					//308
	_T("Bond Timeout!"),										_T("焊接超时!"),								//309
	_T("Servo Error!"),											_T("伺服系统错误!"),							//310
	_T("Spark Failure!"),										_T("电子打火错误!"),							//311
	_T("1st Bond Nonstick!"),									_T("一焊点不粘!"),								//312
	_T("2nd Bond Nonstick!"),									_T("二焊点不粘!"),								//313
	_T("Power Delay (ms)"),										_T("超声延时 (ms)"),							//314
	_T("Tail Shift (um)"),										_T("线尾滑移 (um)"),							//315
	_T("Offload Delay:"),										_T("卸载延时:"),								//316
	_T("Kicker:"),												_T("踢片:"),									//317
	_T("Holder:"),												_T("支撑:"),									//318
	_T("Pusher"),												_T("推动器"),									//319
	_T("Cylinder"),												_T("气缸"),										//320
	_T("Bond Calculate Error!"),								_T("计算错误!"),								//321
	_T("Pusher Stepper"),										_T("推动器步进电机"),							//322
	_T("Start Position:"),										_T("起始位置:"),								//323
	_T("Push"),													_T("推动"),										//324
	_T("Back"),													_T("返回"),										//325
	_T("Onload Stepper"),										_T("上料步进电机"),								//326
	_T("LF Thickness:"),										_T("支架厚度:"),								//327
	_T("Push Distance:"),										_T("推动距离:"),								//328
	_T("Calibration factor out of range"),						_T("校准系数超出有效范围!"),					//329
	_T("Please calibrate again"),								_T("请重新校准!"),								//330
	_T("Neck Height (um)"),										_T("线颈高度 (um)"),							//331
	_T("Operator"),												_T("操作工"),									//332
	_T("Process"),												_T("技术员"),									//333
	_T("Service"),												_T("维护员"),									//334
	_T("Wcl Loop Peak Close (ms/2)"),							_T("线夹弧顶关闭 (ms/2)"),						//335
	_T("Heater temperature out of range!"),						_T("加热器温度超出有效范围!"),					//336
	_T("Claw"),													_T("抓片"),										//337
	_T("Limit"),												_T("推动界限"),									//338
	_T("Trajectory Factor (%)"),								_T("线弧轨迹系数 (%)"),							//339
	_T("BSD Network Error"),									_T("BSD网络错误"),								//340
	_T("BSD Socket Error"),										_T("BSD协议错误"),								//341
	_T("BSD Communication Error"),								_T("BSD通讯错误"),								//342
	_T("BSD Version Error"),									_T("BSD版本错误"),								//343
	_T("EFO Init Error"),										_T("EFO初始化错误"),							//344
	_T("Stick Detection Configure"),							_T("焊点不粘检测配置"),							//345
	_T("Hardware Parameters"),									_T("硬件参数"),									//346
	_T("Apply Voltage (V):"),									_T("施加电压 (V):"),							//347
	_T("Die Detect Threshold (V):"),							_T("芯片检测阈值 (V):"),						//348
	_T("Lead Detect Threshold (V):"),							_T("管脚检测阈值 (V):"),						//349
	_T("Tail Short Threshold:"),								_T("线尾太短阈值:"),							//350
	_T("Nonstick Threshold:"),									_T("焊点不粘阈值:"),							//351
	_T("Bond Detect Parameters"),								_T("焊点检测参数"),								//352
	_T("Tail Too Short!"),										_T("线尾太短!"),								//353
	_T("Pre Clamp Position:"),									_T("预加紧位置:"),								//354
	_T("Lead Frame Index Time Out!"),							_T("支架上料超时!"),							//355
	_T("Update"),												_T("更新"),										//356
	_T("Thread Wire Position"),									_T("穿线位置"),									//357
	_T("Clear Track"),											_T("清空轨道"),									//358
	_T("Skip"),													_T("跳过"),										//359
	_T("Onload Push Kicker Wrong!"),							_T("上料推片器出错!"),							//360
	_T("Onload Picker Jam!"),									_T("上料机械手卡住!"),							//361
	_T("Onload Pully Jam!"),									_T("上料料盒推动器卡住!"),						//362
	_T("Claw Missing Steps, Please Home it!"),					_T("拨爪丢步，请重新复位!"),					//363
	_T("Offload Kicker Wrong!"),								_T("下料踢片出错!"),							//364
	_T("Offload Kicker Jam!"),									_T("下料踢片卡住!"),							//365
	_T("Offload Claw Wrong!"),									_T("下料抓片出错!"),							//366
	_T("Offload Claw Jam!"),									_T("下料抓片卡住!"),							//367
	_T("Offload Inner Pusher Wrong!"),							_T("下料内侧推动器出错!"),						//368
	_T("Offload Pusher Jam!"),									_T("下料推动器卡住!"),							//369
	_T("Offload Outer Pusher Wrong!"),							_T("下料外侧推动器出错!"),						//370
	_T("Offload Outer Pusher Jam!"),							_T("下料外侧推动器卡住!"),						//371
	_T("Offload Inner Holder Wrong!"),							_T("下料内侧支撑出错!"),						//372
	_T("Offload Inner Holder Jam!"),							_T("下料内侧支撑卡住!"),						//373
	_T("Offload Outer Holder Wrong!"),							_T("下料外侧支撑出错!"),						//374
	_T("Offload Outer Holder Jam!"),							_T("下料外侧支撑卡住!"),						//375
	_T("Control"),												_T("控制参数"),									//376
	_T("Pre Impact Force (gm)"),								_T("前置预备压力 (gm)"),						//377	//V1.1.W149 edit
	_T("Advanced Loop"),										_T("高级线弧"),									//378
	_T("Search Delay (ms)"),									_T("搜索延时 (ms)"),							//379
	_T("Pre Heat Time:"),										_T("预热时间:"),								//380
	_T("PR Pass % (Die/Lead)"),									_T("PR及格率 % (芯片/管脚)"),					//381
	_T("Reset"),												_T("清除纪录"),									//382
	_T("Machine ID"),											_T("机器识别号"),								//383	//V1.1.W258 edit
	_T("Start Time:"),											_T("开始时间:"),								//384
	_T("Output:"),												_T("产量:"),									//385
	_T("Production:"),											_T("生产时间:"),								//386
	_T("Standby:"),												_T("待机时间:"),								//387
	_T("Assist"),												_T("辅助次数"),									//388	//v1.5.T71 edit
	_T("Failure"),												_T("失败次数"),									//389	//v1.5.T71 edit
	_T("Wire:"),												_T("打线数:"),									//390
	_T("Tool Usage:"),											_T("瓷咀用量:"),								//391
	_T("EFO Open:"),											_T("打火开路:"),								//392
	_T("Short Tail:"),											_T("线尾过短:"),								//393
	_T("Assistance"),											_T("辅助统计"),									//394
	_T("Threshold"),											_T("阈值"),										//395
	_T("Reference Information"),								_T("参照系信息"),								//396
	_T("Contact Parameters"),									_T("接触面搜索参数"),							//397
	_T("Vision Factors"),										_T("图像分辨率系数"),							//398
	_T("Setting"),												_T("设置"),										//399
	_T("Magnification"),										_T("放大倍数:"),								//400
	_T("Pattern Size X"),										_T("模板尺寸X:"),								//401
	_T("Pattern Size Y"),										_T("模板尺寸Y:"),								//402
	_T("Time (ms)"),											_T("时间 (ms)"),								//403
	_T("Power (Dac)"),											_T("能量 (Dac)"),								//404
	_T("Continue"),												_T("继续"),										//405
	_T("Teach Alignment Point"),								_T("指定定位点"),								//406
	_T("Reference Number"),										_T("参照系数量"),								//407
	_T("Lens Setting"),											_T("镜头设置"),									//408
	_T("Die Number"),											_T("芯片数量"),									//409
	_T("Pattern Parameters"),									_T("模板参数"),									//410
	_T("Vision Algorithm"),										_T("图像识别算法"),								//411
	_T("Locate Mode"),											_T("定位模式"),									//412
	_T("Grey Level"),											_T("灰度图像"),									//413
	_T("Binary"),												_T("黑白图像"),									//414
	_T("Dual Point"),											_T("两点定位"),									//415
	_T("Single Point"),											_T("单点定位"),									//416
	_T("Standard"),												_T("标准"),										//417
	_T("Flat"),													_T("平弧"),										//418
	_T("Mode A"),												_T("模式 A"),									//419
	_T("Mode B"),												_T("模式 B"),									//420
	_T("Bond Mode"),											_T("焊线模式"),									//421
	_T("Bond Control"),											_T("焊线控制"),									//422
	_T("Motor"),												_T("马达"),										//423
	_T("System"),												_T("系统"),										//424
	_T("New"),													_T("新建"),										//425
	_T("Edit"),													_T("编辑"),										//426
	_T("Delete"),												_T("删除"),										//427
	_T("Insert"),												_T("插入"),										//428
	_T("Teach Wire Layout"),									_T("指定连线布局"),								//429
	_T("Wire Property"),										_T("连线属性"),									//430
	_T("1st Bond On"),											_T("一焊点位于"),								//431
	_T("2nd Bond On"),											_T("二焊点位于"),								//432
	_T("Wire Layout"),											_T("连线布局"),									//433
	_T("Wire Num"),												_T("线号"),										//434
	_T("Sequence Setting"),										_T("焊线顺序设置"),								//435
	_T("Program Type"),											_T("程式类型"),									//436
	_T("No. of Row/Col"),										_T("行数/列数"),								//437	//v1.5.T91 edit
	_T(""),														_T(""),											//438	//v1.5.T91 edit
	_T("PR Ahead Col"),											_T("预定位列数"),								//439
	_T("Bond Sequence"),										_T("焊线顺序"),									//440	//v1.5.T91 edit
	_T("Track Type"),											_T("导轨类型"),									//441
	_T("Unit Layout"),											_T("单元布局"),									//442
	_T("None"),													_T("无"),										//443
	_T("Matrix"),												_T("矩阵"),										//444
	_T("Hybrid"),												_T("合成"),										//445
	_T("Die Ahead"),											_T("芯片预定位"),								//446
	_T("Die Lead Ahead"),										_T("芯片管脚预定位"),							//447
	_T("General Profile"),										_T("通用配置"),									//448
	_T("Thread Wire Open Force"),								_T("穿线张开力"),								//449
	_T("Thread Wire Close Force"),								_T("穿线夹紧力"),								//450
	_T("Hold Wire Close Force"),								_T("空置夹紧力"),								//451
	_T("Bond Profile"),											_T("焊线配置"),									//452
	_T("Profile Type"),											_T("配置类型"),									//453
	_T("1st Step Force"),										_T("第一步加力"),								//454
	_T("1st Step Ramp Time"),									_T("第一步转换时间"),							//455
	_T("1st Step Hold Time"),									_T("第一步持续时间"),							//456
	_T("2nd Step Force"),										_T("第二步加力"),								//457
	_T("2nd Step Ramp Time"),									_T("第二步转换时间"),							//458
	_T("2nd Step Hold Time"),									_T("第二步持续时间"),							//459
	_T("Clamp Force"),											_T("夹紧力"),									//460
	_T("Clamp Ramp Time"),										_T("夹紧力转换时间"),							//461
	_T("Ball Sit Open/Close"),									_T("球位置调整张开/夹紧"),						//462
	_T("Search Height Open"),									_T("搜索高度张开"),								//463
	_T("Peak Height Close"),									_T("弧顶高度夹紧"),								//464
	_T("Tail Height Close"),									_T("线尾高度夹紧"),								//465
	_T("Stop All"),												_T("急停"),										//466
	_T("Parameter out of range!"),								_T("参数超出范围!"),							//467
	_T("Software Version"),										_T("软件版本"),									//468
	_T("Wcl 1st Bond Open (ms/2)"),								_T("线夹一焊张开 (ms/2)"),						//469
	_T("Wcl 2nd Bond Open (ms/2)"),								_T("线夹二焊张开 (ms/2)"),						//470
	_T("WCL Loop Peak Close Time (ms)"),						_T("线夹弧顶夹紧时间 (ms)"),					//471
	_T("Onload Fork Not Exist!"),								_T("上料料叉不在!"),							//472
	_T("Offload Fork Not Exist!"),								_T("下料料叉不在!"),							//473
	_T("Offload Outer Fork Not Exist!"),						_T("下料外侧料叉不在!"),						//474
	_T("Offload Fork Full!"),									_T("下料料叉已满!"),							//475
	_T("Offload Outer Fork Full!"),								_T("下料外侧料叉已满!"),						//476
	_T("Gap exist between Lf to Lf!"),							_T("支架之间有间隙！"),							//477
	_T("Image Transfer Time (ms):"),							_T("图像传输时间(ms):"),						//478
	_T("Disable"),												_T("不用"),										//479
	_T("Enable"),												_T("使用"),										//480
	_T("Track Flatness"),										_T("导轨水平度"),								//481
	_T("Press Start to check track flatness"),					_T("按'开始'健检查导轨水平度"),					//482
	_T("Move table to inner track left position"),				_T("移动工作台到内侧导轨左侧位置"),				//483
	_T("Move table to inner track middle position"),			_T("移动工作台到内侧导轨中间位置"),				//484
	_T("Move table to inner track right position"),				_T("移动工作台到内侧导轨右侧位置"),				//485
	_T("Move table to outer track left position"),				_T("移动工作台到外侧导轨左侧位置"),				//486
	_T("Move table to outer track middle position"),			_T("移动工作台到外侧导轨中间位置"),				//487
	_T("Move table to outer track right position"),				_T("移动工作台到外侧导轨右侧位置"),				//488
	_T("Track level list:"),									_T("导轨高度列表:"),							//489
	_T("BSD"),													_T("不粘检测"),									//490
	_T("Scrub"),												_T("研磨"),										//491
	_T("Scrub Group"),											_T("研磨组别"),									//492
	_T("Scrub Offset1 (um)"),									_T("研磨偏移1 (um)"),							//493
	_T("Scrub Offset2 (um)"),									_T("研磨偏移2 (um)"),							//494
	_T("Cycle"),												_T("循环次数"),									//495
	_T("Amplitude (um)"),										_T("幅度 (um)"),								//496
	_T("Direction"),											_T("方向"),										//497
	_T("Speed (%)"),											_T("速度 (%)"),									//498
	_T("Vertical"),												_T("纵向"),										//499
	_T("Auto Skip Die/Unit"),									_T("自动跳过芯片/单元"),						//500
	_T("Search Range"),											_T("搜索范围"),									//501
	_T("Bond Process Configure"),								_T("焊接过程配置"),								//502
	_T("Safety"),												_T("安全控制"),									//503
	_T("Capillary Wear Out Limit:"),							_T("瓷咀磨损极限:"),							//504
	_T("Capillary Usage"),										_T("瓷咀使用情况"),								//505
	_T("Limit:"),												_T("极限:"),									//506
	_T("Count"),												_T("次数"),										//507	//v1.5.T71 edit
	_T("Capillary exceed Wear Out Limit"),						_T("瓷咀磨损超出极限"),							//508
	_T("Please change capillary"),								_T("请更换瓷咀"),								//509
	_T("Red"),													_T("红光"),										//510
	_T("Side Light"),											_T("侧光"),										//511	//V1.1.W133 edit
	_T("Blue"),													_T("蓝光"),										//512
	_T("Force"),												_T("压力"),										//513
	_T("Confirm"),												_T("确认"),										//514
	_T("Tail Length"),											_T("线尾长度"),									//515
	_T("Current Fire Level"),									_T("当前烧球高度"),								//516
	_T("New Fire Level"),										_T("新烧球高度"),								//517
	_T("This wire is bonded"),									_T("当前线已经焊过,"),							//518
	_T("Please confirm to continue or not"),					_T("请确认是否继续"),							//519
	_T("Trajectory Mode"),										_T("线弧轨迹模式"),								//520
	_T("All"),													_T("全部"),										//521
	_T("Odd"),													_T("奇数"),										//522
	_T("Even"),													_T("偶数"),										//523
	_T("AUX PR"),												_T("模板备份"),									//524
	_T("Alignment"),											_T("定位点"),									//525
	_T("Bond Position"),										_T("焊点"),										//526
	_T("Onload Fork Empty!"),									_T("上料盒空!"),								//527
	_T("Please Onload Lead Frame!"),							_T("请填装新支架!"),							//528
	_T("Bond Time 1"),											_T("焊接时间一"),								//529
	_T("Bond Power 1"),											_T("焊接能量一"),								//530
	_T("Bond Force 1"),											_T("焊接压力一"),								//531
	_T("Bond Time 2"),											_T("焊接时间二"),								//532
	_T("Bond Power 2"),											_T("焊接能量二"),								//533
	_T("Bond Force 2"),											_T("焊接压力二"),								//534
	_T("Fork Capacity"),										_T("下料盒容量:"),								//535
	_T("Inner Track Offload Fork Full!"),						_T("内侧轨道下料盒已满!"),						//536
	_T("Please Offload Lead Frame!"),							_T("请卸载支架!"),								//537
	_T("Outer Track Offload Fork Full!"),						_T("外侧轨道下料盒已满!"),						//538
	_T("Shift Speed (%)"),										_T("滑移速度(%)"),								//539
	_T("Coax Light"),											_T("同轴光"),									//540	//V1.1.W133 edit
	_T("Die Tol Fail"),											_T("芯片定位间距超出"),							//541
	_T("Lead Tol Fail"),										_T("管脚定位间距超出"),							//542
	_T("Alignment Tolerance"),									_T("定位间距阈值"),								//543
	_T("Redo PR"),												_T("再定位"),									//544
	_T("(0) Disable (1) Enable"),								_T("(0) 不检测 (1) 检测"),						//545
	_T("(0) Wire (1) BBOS (2) BSOB (3) BSB"),					_T("(0) 不加球 (1) 压球 (2) 垫球 (3) 压垫球"),	//546	//v3.1.T90 edit
	_T("Bump Ball"),											_T("加球"),										//547
	_T("BSOB Parameter"),										_T("垫球参数"),									//548
	_T("Shape"),												_T("球形"),										//549
	_T("Bond Ball Mode"),										_T("加球形式"),									//550
	_T("BBOS"),													_T("压球"),										//551	//v3.0.T429 rename
	_T("BSOB"),													_T("垫球"),										//552	//v3.0.T429 rename
	_T("2nd Bond Offset (um)"),									_T("二焊偏移 (um)"),							//553
	_T("BSOB Base Height (um)"),								_T("垫球球基高度 (um)"),						//554
	_T("BSOB Motion Speed (%)"),								_T("垫球运动速度 (%)"),							//555
	_T("BSOB Scrub Distance (um)"),								_T("垫球刮研距离 (um)"),						//556
	_T("BSOB Span Length (um)"),								_T("垫球斜跨长度 (um)"),						//557
	_T("BSOB Ball Thickness (um)"),								_T("垫球球形厚度 (um)"),						//558
	_T("Bond Sequence Error!"),									_T("焊线次序错误!"),							//559
	_T("Wire and Ball"),										_T("线和球"),									//560
	_T("Ball Only"),											_T("球"),										//561
	_T("Wire Only"),											_T("线"),										//562
	_T("Bond Object"),											_T("焊接对象"),									//563
	_T("Bump Wire"),											_T("焊线参数"),									//564
	_T("Bump Ball"),											_T("焊球参数"),									//565
	_T("No Alignment"),											_T("没有定位"),									//566
	_T("Press 'Start' to teach contact level"),					_T("按'开始'指定接触面高度"),					//567
	_T("Teach contact level of "),								_T("搜寻接触高度: "),							//568
	_T("Loop Top Mode"),										_T("弧顶模式"),									//569
	_T("Please index leadframe manually"),						_T("请手动跳格支架"),							//570
	_T("USG"),													_T("超声"),										//571
	_T("USG Ramp Time"),										_T("超声转换时间"),								//572
	_T("Thermal Expansion"),									_T("热膨胀"),									//573
	_T("Expansion Handle:"),									_T("膨胀处理:"),								//574
	_T("Handle Time:"),											_T("处理时间:"),								//575
	_T("Pre Heater"),											_T("预加热器"),									//576
	_T("Sensor Switch Time"),									_T("感应器转换时间"),							//577
	_T("Onload Inner Track no leadframe"),						_T("上料内侧轨道无支架"),						//578
	_T("Onload Outer Track no leadframe"),						_T("上料外侧轨道无支架"),						//579
	_T("Track leadframe not exist"),							_T("轨道支架不存在"),							//580
	_T("Reset"),												_T("复位"),										//581
	_T("Onload Track Standby"),									_T("上料轨道预备时间"),							//582
	_T("Bond Track Standby"),									_T("焊接轨道预备时间"),							//583
	_T("Onload track jam"),										_T("上料轨道支架卡住"),							//584
	_T("Onload outer track jam"),								_T("上料外侧轨道支架卡住"),						//585
	_T("Impact Force 2"),										_T("冲击压力二"),								//586
	_T("Feed Power"),											_T("进线能量"),									//587
	_T("Current:"),												_T("当前线:"),									//588
	_T("Restart:"),												_T("开始线:"),									//589
	_T("Total Wire:"),											_T("总线数:"),									//590
	_T("NSOL Sample:"),											_T("二焊检测读数:"),							//591
	_T("Heater"),												_T("加热器"),									//592
	_T("Bond Para"),											_T("焊接参数"),									//593
	_T("Loop Para"),											_T("线弧参数"),									//594
	_T("Wire Usage"),											_T("用线量"),									//595
	_T("Wires:"),												_T("焊线量:"),									//596
	_T("Software Manager"),										_T("软件管理"),									//597
	_T("Upgrade"),												_T("升级软件"),									//598
	_T("Current Version"),										_T("当前版本"),									//599
	_T("Date Time"),											_T("日期时间"),									//600
	_T("Software Upgrade"),										_T("软件升级"),									//601
	_T("Restart Setting"),										_T("重启动设置"),								//602
	_T("Bonder"),												_T("焊接系统"),									//603
	_T("MHS"),													_T("过料系统"),									//604
	_T("Servo"),												_T("伺服系统"),									//605
	_T("Cold Start Items:"),									_T("冷启动选项:"),								//606
	_T("Press Edit to edit wire bond point"),					_T("按'编辑'键编辑焊点位置"),					//607
	_T("Press New to teach new wire bond point"),				_T("按'新线'键学习焊点位置"),					//608
	_T("Press Start to edit program"),							_T("按'开始'键编辑程式"),						//609
	_T("Press Prev/Next to change wire no."),					_T("按'前一步','下一步'改变线号"),				//610
	_T("Bond Head"),											_T("焊头"),										//611
	_T("Safe Position"),										_T("安全位置"),									//612
	_T("Load Control Parameter"),								_T("加载控制参数"),								//613
	_T("Position"),												_T("位置"),										//614
	_T("Load"),													_T("加载"),										//615
	_T("Engineer"),												_T("工程师"),									//616
	_T("PR Pattern"),											_T("PR模板"),									//617
	_T("Wire"),													_T("布线"),										//618
	_T("Range"),												_T("参数范围"),									//619
	_T("Lower"),												_T("下限"),										//620
	_T("Upper"),												_T("上限"),										//621
	_T("Vision"),												_T("视觉参数"),									//622
	_T("Light"),												_T("灯光参数"),									//623
	_T("Wire roll gold wire use up, please check！"),			_T("线轴金线用完，请检查！"),					//624
	_T("Are you sure to delete current bond program record?"),	_T("确认删除当前程式？"),						//625
	_T("Coax Blue"),											_T("蓝色同轴光"),								//626
	_T("Coax Red"),												_T("红色同轴光"),								//627
	_T("Side Blue"),											_T("蓝色侧光"),									//628
	_T("Side Red"),												_T("红色侧光"),									//629
	_T("Threshold"),											_T("阈值"),										//630
	_T(">>"),													_T(">>"),										//631
	_T("<<"),													_T("<<"),										//632  
	_T("Material Handler"),										_T("过料系统"),									//633
	_T("PreHeat"),												_T("夹紧加热"),									//634
	_T("BSD Debug"),											_T("BSD 检测调试"),								//635
	_T("BSD Debug Info Record"),								_T("BSD 检测信息保留"),							//636
	_T("Recommended BSD Parameters"),							_T("BSD 推荐参数导出"),							//637
	_T("Please Confirm no Error on previous 5 units!"),			_T("如前5焊点无误，请按确认！"),				//638
	_T("Apply"),												_T("导入参数"),									//639
	_T("Clear All"),											_T("清空全部"),									//640
	_T("AutoSample On"),										_T("开启自动采样"),								//641
	_T("Fake False Alarm Detected,press ok to auto sampling"),	_T("可能出现误报，确认开启自动采样？"),			//642
	_T("Die Detect:(V):"),										_T("芯片检测阈值 (V):"),						//643
	_T("Lead Detect: (V):"),									_T("管脚检测阈值 (V):"),						//644
	_T("Tail Short:"),											_T("线尾太短阈值:"),							//645
	_T("Nonstick:"),											_T("焊点不粘阈值:"),							//646
	_T("1st Power:"),											_T("1焊能量:"),									//647
	_T("2nd Power:"),											_T("2焊能量:"),									//648
	_T("1st Force :"),											_T("1焊压力:"),									//649
	_T("2nd Force:"),											_T("2焊压力:"),									//650
	_T("Program:"),												_T("程式:"),									//651
	_T("1st Bond Position:"),									_T("预设一焊坐标:"),							//652
	_T("2nd Bond Position:"),									_T("预设二焊坐标:"),							//653
	_T("Recipe:"),												_T("程式："),									//654
	_T("No Program"),											_T("程式未加载"),								//655
	_T("Loop"),													_T("线弧"),										//656
	_T("Neck Height Pre-Move (%)"),								_T("线颈高度提前移动 (%)"),						//657
	_T("Neck Distance Pre-Move (%)"),							_T("线颈距离提前移动 (%)"),						//658
	_T("Kink2 Height Pre-Move (%)"),							_T("拐点2高度提前移动 (%)"),					//659	//v3.1.T320 edit
	_T("Kink2 Distance Pre-Move (%)"),							_T("拐点2距离提前移动 (%)"),					//660	//v3.1.T320 edit
	_T("Loop Top Pre-Move (%)"),								_T("弧顶提前移动 (%)"),							//661
	_T("PR Version"),											_T("PR 版本"),									//662
	_T("No Elec Power.Auto shut down in 5 sec"),				_T("无市电，本程序在5秒内退出！"),				//663 
	_T("Last LF"),												_T("最后支架"),									//664
	_T("1st Bond Boost"),										_T("一焊助推"),									//665
	_T("Duration (Impact/Base)"),								_T("助推时间 (冲击/焊接)"),						//666
	_T("Mode (Impact/Base)"),									_T("助推模式 (冲击/焊接)"),						//667
	_T("Amplitude (Impact/Base)"),								_T("幅度 (冲击/焊接)"),							//668
	_T("Short"),												_T("短"),										//669
	_T("Long"),													_T("长"),										//670
	_T("Frequency"),											_T("谐振频率"),									//671
	_T("Impedance"),											_T("谐振阻抗"),									//672
	_T("Transducer"),											_T("超声校准"),									//673
	_T("Seat (Dac/ms)"),										_T("坐线(Dac/ms)"),								//674
	_T("Feed (Dac/ms)"),										_T("进线(Dac/ms)"),								//675
	_T("Tail (Dac/ms)"),										_T("做尾(Dac/ms)"),								//676 
	_T("Seat (ms):"),											_T("坐线(ms):"),								//677
	_T("Feed (ms):"),											_T("进线(ms):"),								//678
	_T("Tail (ms):"),											_T("做尾(ms):"),								//679
	_T("USG Error occur，Please check calibration first!:"),		_T("超声输出错误，请先检查校准状态！"),		//680
	_T("Time:"),												_T("时间:"),									//681
	_T("Level:"),												_T("大小："),									//682
	_T("Generate"),												_T("输出"),										//683
	_T("Voltage"),												_T("恒压"),										//684
	_T("Current"),												_T("恒流"),										//685
	_T("Power"),												_T("恒功率"),									//686
	_T("Mode"),													_T("模式"),										//687
	_T("USG Setup"),											_T("超声输出设定"),								//688
	_T("High Frequency"),										_T("高频校准"),									//689
	_T("Low Frequency"),										_T("低频校准"),									//690
	_T("USG Output Generate "),									_T("超声输出"),									//691
	_T("USG Calibrate Success!"),								_T("超声校准成功！"),							//692
	_T("Impedance Out of Range"),								_T("阻抗超出范围！"),							//693
	_T("USG Calibrate Failed!"),								_T("超声校准失败！"),							//694	//v3.1.T1 edit
	_T("File"),													_T("文件管理"),									//695
	_T("Diagnostic Tool"),										_T("诊断工具"),									//696
	_T("Copper Sequence"),										_T("铜线顺序"),									//697
	_T("Lead Frame Information"),								_T("支架信息"),									//698
	_T("Total Units"),											_T("支架单元数"),								//699
	_T("Index Times"),											_T("进料次数"),									//700
	_T("Onload Picker"),										_T("上料抓手"),									//701 
	_T("Injector Stepper"),										_T("进料器"),									//702
	_T("Offload Kicker"),										_T("下料推手"),									//703
	_T("Material Handling System Utility"),						_T("原料处理系统调试"),							//704
	_T("Left Claw"),											_T("左侧拨爪"),									//705
	_T("Right Claw"),											_T("右侧拨爪"),									//706
	_T("Compensation"),											_T("位置补偿"),									//707
	_T("Clamper"),												_T("夹具控制"),									//708
	_T("SingleIndex"),											_T("单步进料"),									//709
	_T("Lead PR:"),												_T("管脚识别:"),								//710
	_T("Contact Level Search"),									_T("接触面接触测试"),							//711
	_T("Press Start to Begin a New Test"),						_T("按开始进行新一组测试"),						//712
	_T("Onload"),												_T("上料"),										//713
	_T("Track Front"),											_T("导轨前视"),									//714
	_T("Track Back"),											_T("导轨后视"),									//715
	_T("Sensor & Cylinder "),									_T("传感器及气动件"),							//716
	_T("Stepper"),												_T("步进马达"),									//717
	_T("Onload Track Double Lead Frame!"),						_T("上料轨道双支架!"),							//718
	_T("LF standby Unit:"),										_T("初始支架单元:"),							//719
	_T("Left Claw to Bond Area Index Times:"),					_T("左拨爪至焊接点跳格次数:"),					//720
	_T("Bond Area to Right Claw Index Times:"),					_T("焊接点至右拨爪跳格次数:"),					//721
	_T("Preheat Delay(ms):"),									_T("预加热延时(ms):"),							//722
	_T("Clamp Open Pre-Trigger Claw (%):"),						_T("夹具打开预触发拨爪(%):"),					//723
	_T("Claw Pre-Trigger Clamp Close (%):"),					_T("拨爪预触发夹具关闭(%):"),					//724
	_T("Claw Catch Up:"),										_T("追赶速度:"),								//725
	_T("Index:"),												_T("支架索引"),									//726
	_T("Index Speed"),											_T("索引速度"),									//727
	_T("Double Check Position:"),								_T("双支检测位置:"),							//728
	_T("Inject Position:"),										_T("支架推进位置:"),							//729
	_T("CheckPos"),												_T("检测位"),									//730
	_T("Inject"),												_T("推进位"),									//731
	_T("Run With Sensors"),										_T("传感器检测"),								//732
	_T("Stepper Time Test"),									_T("步进时间测试"),								//733
	_T("Sensor"),												_T("传感器"),									//734
	_T("Index Cycle Time (ms):"),								_T("单次索引时间(ms):"),						//735
	_T("Exposure Time (us):"),									_T("曝光时间(us):"),							//736
	_T("Feature"),												_T("特性"),										//737
	_T("Constant Wire Length:"),								_T("线长固定:"),								//738
	_T("Table"),												_T("工作台"),									//739
	_T("X Limit"),												_T("X限位"),									//740
	_T("Y Limit"),												_T("Y限位"),									//741
	_T("Will Start to Search Table Limit,"),					_T("即将搜索工作台限位,"),						//742
	_T("Please Make Sure Bond Area Clear!"),					_T("请确认焊线区域无阻挡!"),					//743
	_T("Search Table Limit Finish!"),							_T("搜索工作台限位完成!"),						//744
	_T("Position compensation in progress..."),					_T("位置补偿进行中..."),						//745
	_T("Position compensation success!"),						_T("位置补偿成功!"),							//746
	_T("Position compensation out of range!"),					_T("位置补偿超出范围!"),						//747
	_T("Position compensation fail!"),							_T("位置补偿失败!"),							//748
	_T("Press 'Bond' to make capillary mark "),					_T("按'焊接'压出瓷咀印"),						//749	//v3.0.T454 edit
	_T("Press 'Verify' to check capillary mark"),				_T("按'测试'检查瓷咀印"),						//750
	_T("Bond"),													_T("焊接"),										//751
	_T("OK"),													_T("矫正"),										//752 
	_T("Press 'OK' to continue"),								_T("按'OK'继续"),								//753 
	_T("PR Retry"),												_T("PR再试"),									//754
	_T("Please Update Contact Level!"),							_T("请更新接触面高度!"),						//755
	_T("Light"),												_T("灯光"),										//756 
	_T("Index"),												_T("进料"),										//757 
	_T("Adjust"),												_T("微调"),										//758 
	_T("Thread"),												_T("穿线"),										//759
	_T("Oscillate"),											_T("振动"),										//760 
	_T("Zero BTOS"),											_T("零瓷咀偏移"),								//761
	_T("AFP Mode"),												_T("装机工具"),									//762
	_T("IO"),													_T("输入输出"),									//763
	_T("Encoder"),												_T("编码器"),									//764
	_T("Auto Tune"),											_T("自动调节"),									//765
	_T("Fork Switch Buffer"),									_T("下料区分容量:"),							//766
	_T("Remove Window Clamper"),								_T("取下夹具"),									//767
	_T("PR Fail!"),												_T("图像无法识别！"),							//768
	_T("Material Handler Calibration"),							_T("过料系统校准"),								//769
	_T("PR Index"),												_T("进料图像学习"),								//770
	_T("Device"),												_T("设备参数"),									//771
	_T("Setting"),												_T("配置参数"),									//772
	_T("Bond Unit"),											_T("焊接单元"),									//773
	_T("Sensor to Bond Area Units:"),							_T("最后支架偏移量："),							//774
	_T("FAB Test"),												_T("FAB 测试"),									//775
	_T("Thermal Protection Time:"),								_T("加热保护时间:"),							//776
	_T("Correct Bond Offset"),									_T("补线附加焊接参数"),							//777
	_T("Power Compensation"),									_T("能量补偿"),									//778
	_T("Contact Level Offset"),									_T("接触面偏移量"),								//779
	_T("Circle Size"),											_T("焊点大小"),									//780
	_T("Bump Ball Control"),									_T("加球控制"),									//781
	_T("File Name Too Long!"),									_T("文件名过长!"),								//782
	_T("Indexer"),												_T("抓片器"),									//783
	_T("Test"),													_T("测试"),										//784
	_T("Database"),												_T("材料选择"),									//785
	_T("Magazine"),												_T("料盒"),										//786
	_T("Leadframe"),											_T("支架"),										//787
	_T("Pitch:"),												_T("列间距:"),									//788
	_T("Bottom to 1st Slot:"),									_T("底沿高:"),									//789
	_T("Last Slot to Top:"),									_T("顶沿高:"),									//790
	_T("Edge length:"),											_T("边沿宽:"),									//791
	_T("Clamp Column"),											_T("夹具列数"),									//792
	_T("1st Heat Delay (ms)"),									_T("初始预加热延时(ms)"),						//793
	_T("Heat Delay (ms)"),										_T("预加热延时(ms)"),							//794
	_T("PR Index"),												_T("进料图像"),									//795
	_T("PR Index Tol (um)"),									_T("进料图像阈值(um)"),							//796
	_T("Srch Back Length"),										_T("重新搜索距离"),								//797
	_T("Standby Length (mm)"),									_T("进料等待位置(mm)"),							//798
	_T("Eject Speed"),											_T("支架推出速度"),								//799
	_T("Right Clearance (mm)"),									_T("右极限阈值(mm)"),							//800
	_T("Slowest"),												_T("很慢"),										//801
	_T("Slower"),												_T("较慢"),										//802
	_T("Slow"),													_T("慢"),										//803
	_T("Medium"),												_T("中等"),										//804
	_T("Fast"),													_T("快"),										//805
	_T("Faster"),												_T("较快"),										//806
	_T("Fastest"),												_T("最快"),										//807
	_T("Preclamp Position:"),									_T("预夹紧位置："),								//808
	_T("Preclamp"),												_T("预夹紧"),									//809
	_T("Remove"),												_T("移除夹具"),									//810
	_T("Onload 1st Slot:"),										_T("上料第一槽："),								//811
	_T("Slot Pitch:"),											_T("槽间距："),									//812
	_T("Slot Position Fine Tune"),								_T("槽间距微调"),								//813
	_T("Swap Pitch:"),											_T("两料盒间距："),								//814
	_T("Next Slot"),											_T("下一槽"),									//815
	_T("Indexer Alignment"),									_T("抓片器位置校准"),							//816
	_T("Indexer Right Limit"),									_T("抓片器右极限"),								//817
	_T("Indexer Logic Zero"),									_T("抓片器逻辑零点"),							//818
	_T("Index Tune"),											_T("索引偏移量调节"),							//819
	_T("Holding Fork"),											_T("升降料叉"),									//820
	_T("Pully Motor"),											_T("推动器"),									//821
	_T("Taught Pos"),											_T("学习位置"),									//822
	_T("Left Claw:"),											_T("左线圈:"),									//823
	_T("Right Claw:"),											_T("右线圈:"),									//824
	_T("Puller:"),												_T("拉片器:"),									//825
	_T("Onload Air:"),											_T("上料气阀:"),								//826
	_T("Offload Air:"),											_T("下料气阀:"),								//827
	_T("Without LF"),											_T("无支架空跑"),								//828
	_T("Solenoid Test"),										_T("螺线管测试"),								//829
	_T("Please remove all magazines on elevator"),				_T("请移除升降机上所有料盒"),					//830
	_T("Please wait elevator home and standby finish"),			_T("请等待升降机归零位"),						//831
	_T("Please adjust first slot level"),						_T("请调节料盒第一槽位置"),						//832
	_T("Please adjust last slot level"),						_T("请调节料盒最后一槽位置"),					//833
	_T("Please adjust 1st slot level of next magazine"),		_T("请调节第二料盒第一槽位置"),					//834
	_T("Operating side: Onloader"),								_T("操作对象：上料升降机"),						//835
	_T("Operating side: Offloader"),							_T("操作对象：下料升降机"),						//836
	_T("Press 'Confirm' to load recipe, 'Stop' to cancel"),		_T("按‘确认’加载程式，按‘停止’取消"),			//837	//V1.1.W141 edit
	_T("Press 'Done' to quit"),									_T("按'完成'退出"),								//838	//v3.0.T454 edit
	_T("Please wait while indexer homing..."),					_T("请等待抓片器归零..."),						//839
	_T("Please move indexer to Logic Zero position"),			_T("请移动抓片器，使其中点对准轨道中心刻标！"),	//840
	_T("Press Next to search the Right Limit Sensor!"),			_T("按'下一步'抓片器自动搜寻右极限！"),			//841	//V1.1.W139 edit
	_T("Press Next to enter Indexing Offset Adjustment!"),		_T("按'下一步'进入学习跳格偏移量！"),			//842	//V1.1.W139 edit
	_T("Please ensure no leadframe on track!"),					_T("请确保轨道上无支架！"),						//843
	_T("Press 'Next' to index, then tune position"),			_T("按'下一步'跳格支架，然后调节位置"),			//844	//V1.1.W139 edit
	_T("Onload Elevator No Magazine!"),							_T("上料升降台无料盒"),							//845	//V1.1.W136 edit
	_T("Offload Elevator No Magazine!"),						_T("下料升降台无料盒"),							//846	//V1.1.W136 edit
	_T("Search Location Time Out!"),							_T("搜索位置传感器超时！"),						//847
	_T("Leadframe Jamed the Offload Elevator!"),				_T("下料卡料！"),								//848
	_T("Onload Elevator No Response!"),							_T("上料升降台无响应！"),						//849	//V1.1.W136 edit
	_T("Calibration"),											_T("校准"),										//850
	_T("Verification"),											_T("检验"),										//851
	_T("Tail Break"),											_T("线尾拉断"),									//852
	_T("Distance (um)"),										_T("距离(um)"),									//853
	_T("Eject"),												_T("送出料盒"),									//854	//V1.1.W149 edit
	_T("Edit Material Handler Program"),						_T("编辑过料系统程式"),							//855
	_T("Claw Offset (um)"),										_T("进料位置微调(um)"),							//856
	_T("Delete all elevator data and re-teach?"),				_T("确认删除所有升降台数据？"),					//857	//V1.1.W136 edit
	_T("Delete all PR index data and re-teach? "),				_T("确认删除所有进料图像数据？"),				//858
	_T("Please Manually Remove Leadframe On Track!"),			_T("请手动移除导轨上的材料！"),					//859
	_T("Remove Leadframe On Track Automatically ?"),			_T("是否自动移除导轨上的材料？"),				//860
	_T("Open Clamp Delay (ms)"),								_T("夹具打开延时(ms)"),							//861
	_T("Leadframe left on the track last time!"),				_T("检测到上次关机前轨道上有未完成支架"),		//862
	_T("Continue to bond remaining leadframe on track? "),		_T("是否继续上次未完成支架？"),					//863
	_T("Enhance"),												_T("增强"),										//864
	_T("No Lf Run"),											_T("无支架空跑"),								//865
	_T("Teach Material Handler Program"),						_T("学习过料系统程式"),							//866
	_T("Move table to safe position, then press Next"),			_T("移动工作台到安全位置, 然后按'下一步'"),		//867	//V1.1.W139 edit
	_T("Teach Table Safe Position"),							_T("学习XY安全位置"),							//868
	_T("Program Exist"),										_T("已有程式"),									//869
	_T("No Recipe"),											_T("无程式"),									//870
	_T("Teach New"),											_T("新程式"),									//871
	_T("Teach Bond Program"),									_T("学习焊线程式"),								//872
	_T("Edit Bond Program "),									_T("编辑焊线程式"),								//873
	_T("1st LF Preheat Time (ms):"),							_T("第一条支架预热时间(ms):"),					//874
	_T("Normal LF Preheat Time (ms):"),							_T("正常支架预热时间(ms):"),					//875
	_T("Offload Position (1/2):"),								_T("下料位置(1/2):"),							//876
	_T("Position1"),											_T("位置1"),									//877
	_T("Position2"),											_T("位置2"),									//878
	_T("Advanced Vision"),										_T("高级视觉"),									//879
	_T("Rotate Angle:"),										_T("旋转角度:"),								//880
	_T("Shape Recognize"),										_T("形状识别"),									//881
	_T("Impact Force 1"),										_T("冲击压力一"),								//882
	_T("Impact Time 1"),										_T("冲击时间一"),								//883
	_T("Impact Time 2"),										_T("冲击时间二"),								//884
	_T("Impact Power 1"),										_T("冲击能量一"),								//885
	_T("Impact Power 2"),										_T("冲击能量二"),								//886
	_T("Machine is in Standby Mode..."),						_T("机器睡眠模式中..."),						//887
	_T("Click Mouse Left Button to Quit Standby Mode!"),		_T("单击鼠标左键退出睡眠模式！"),				//888
	_T("Enter Machine Standby Mode"),							_T("进入机器睡眠模式"),							//889
	_T("Back-up ID:"),											_T("备份模板号："),								//890
	_T("Arrange the area of BACK-UP PR pattern"),				_T("学习备份图像模板"),							//891
	_T("Press Next to finish"),									_T("按'下一步'结束学习"),						//892	//V1.1.W139 edit
	_T("Press Next to adjust leadframe position"),				_T("按'下一步'调整支架位置"),					//893	//V1.1.W139 edit
	_T("Setup jig, then press capillary to surface"),			_T("请安装校准工具，下压焊头至其表面"),			//894
	_T("Press Next to record Flat Level"),						_T("按'下一步'记录焊头水平位"),					//895	//V1.1.W139 edit
	_T("Release bond head, then remove jig"),					_T("松开焊头然后移除校准工具"),					//896
	_T("Flat Level"),											_T("焊头水平位"),								//897
	_T("Multiple BTOS"),										_T("多瓷咀偏移"),								//898
	_T("Confirm Red Button on the panel is  ON !"),				_T("请确认红色马达开关开启！"),					//899
	_T("Ultra Low"),											_T("超低"),										//900	//V1.1.W149 edit
	_T("Pre Impact Time (ms)"),									_T("前置预备时间 (ms)"),						//901	//V1.1.W149 edit
	_T("Pre Shift 2 (um)"),										_T("预滑移二 (um)"),							//902
	_T("Are you sure to overwrite current device setting?"),	_T("确定更改材料参数？"),						//903
	_T("Please make sure the vision is not expired"),			_T("请检查您的使用权限是否过期！"),				//904
	_T("Debug"),												_T("调试"),										//905
	_T("N2H2 Enable"),											_T("氮气控制"),									//906
	_T("N2H2 Stable Time"),										_T("氮气稳定时间"),								//907
	_T("Waiting N2H2 stable..."),								_T("等待氮气中..."),							//908
	_T("Onload Magazine Pully full"),							_T("请清除已满的上料料盒"),						//909
	_T("Offload Magazine Pully full"),							_T("请清除已满的下料料盒"),						//910
	_T("E-Torch Burn In"),										_T("连续打火调试"),								//911
	_T("Cycle Time"),											_T("周期时间"),									//912	//v1.5.T71 edit
	_T("No. of Cycle"),											_T("周期次数"),									//913	//v1.5.T71 edit
	_T("Are you sure to delete all back-up PR?"),				_T("确认删除所有备份模板？"),					//914
	_T("Are you sure to swap back-up PR to main PR?"),			_T("确认将当前模板用于主模板？"),				//915
	_T("Swap"),													_T("调换"),										//916
	_T("Clean All"),											_T("全删除"),									//917
	_T("Auto Swap Main PR"),									_T("自动交换主模板"),							//918
	_T("Multi-PR"),												_T("多点识别"),									//919
	_T("Adjust Lead BTOS"),										_T("调节管脚瓷咀偏移"),							//920
	_T("Capillary Wear Out Power Compensation"),				_T("瓷咀磨损能量补偿"),							//921
	_T("Start/End Count"),										_T("瓷咀开始/结束用量"),						//922
	_T("Power Comp 1/2 (%)"),									_T("能量补偿 1/2 (%)"),							//923
	_T("FAB2 Tail Length"),										_T("FAB2 线尾长度"),							//924
	_T("BSOB FAB Type"),										_T("垫球 FAB 组别"),							//925	//v3.1.T381 rename
	_T("Horizon"),												_T("横向"),										//926
	_T("Circle"),												_T("圆形"),										//927
	_T("Ball Scrub"),											_T("加球研磨"),									//928
	_T("Contrast"),												_T("对比度"),									//929
	_T("Red Gain"),												_T("红增益"),									//930
	_T("Brightness"),											_T("亮度"),										//931
	_T("Saturation"),											_T("饱和度"),									//932
	_T("PR Filter"),											_T("图像滤镜"),									//933
	_T("Press 'Confirm' to reset, press 'Stop' to cancel"),		_T("按‘确认’清除，按‘停止’取消"),				//934	//V1.1.W261 edit
	_T("Warning On Serial Skip Num"),							_T("连续跳过警告数目"),							//935	//V1.1.W262 edit
	_T("Tail Bond"),											_T("尾焊"),										//936
	_T("Bond Tools"),											_T("焊线工具"),									//937
	_T("Capi Smooth"),											_T("瓷咀磨合"),									//938
	_T("Smooth"),												_T("磨合"),										//939
	_T("Clean"),												_T("清洁"),										//940
	_T("Press 'Start' to home indexer"),						_T("按'开始'归零抓片器"),						//941
	_T("Press 'Start' to teach Index PR"),						_T("按'开始'学习进料图像"),						//942
	_T("Press 'Teach New' to index a new leadframe"),			_T("按'新程式'进一片新支架"),					//943
	_T("Press 'Done' to confirm final position"),				_T("按'完成'确认最后位置"),						//944
	_T("Press 'Start' to teach elevator position"),				_T("按'开始'学习升降台位置"),					//945	//V1.1.W136 edit
	_T("Press 'Next' to skip this step"),						_T("按'下一步'跳过此步"),						//946	//V1.1.W139 edit
	_T("Incomplete Rotate Compensation:"),						_T("拨爪倒转位置补偿："),						//947
	_T("Press 'Continue' to preview bond sequence"),			_T("按'继续'预览焊接顺序"),						//948
	_T("BBOS Parameter"),										_T("压球参数"),									//949
	_T("Hint: If multiple dies in one cup, please input numbers above！"),	_T("提示：如果一个杯中有多个芯片，请在上填入数量！"),	//950
	_T("Hint: Found multiple die in one cup"),					_T("提示：发现一个杯中有多个芯片"),				//951
	_T("Choose correct die on top combo box"),					_T("请在最上方下拉框中选择焊点对应芯片"),		//952	//V1.1.W149 edit
	_T("Self-define"),											_T("焊线范围"),									//953
	_T("Input Bond Start Wire and End Wire Number"),			_T("输入焊接开始及结束线号"),					//954
	_T("Start Wire:"),											_T("开始线号："),								//955
	_T("End Wire:"),											_T("结束线号："),								//956
	_T("Bump Wire Bond Power"),									_T("加球线焊能量"),								//957
	_T("Onloader Push Kicker"),									_T("进料推片器"),								//958
	_T("Offloader Time Out!"),									_T("进下料升降台超时"),							//959	//V1.1.W136 edit
	_T("Are You Sure To Delete Original Recipe"),				_T("请确认要清除原始的程序"),					//960
	_T("And Teach New Recipe?"),								_T("并教新的程序?"),							//961
	_T("Edit Point"),											_T("焊点编辑"),									//962
	_T("Looping Pre-Trigger"),									_T("线弧提前量"),								//963
	_T("Wire Clamp Pre-Trigger"),								_T("线夹提前量"),								//964
	_T("Tail Break Threshold:"),								_T("线尾拉断阈值:"),							//965
	_T("Pre Impact Power 1"),									_T("冲击预备能量一"),							//966
	_T("Pre Impact Power 2"),									_T("冲击预备能量二"),							//967
	_T("BSOB Direction Angle (Deg)"),							_T("垫球方向角度 (度)"),						//968
	_T("Flex"),													_T("弯折"),										//969
	_T("BSOB Bump Shape Mode"),									_T("垫球球形模式"),								//970
	_T("Reverse Motion (um)"),									_T("反向运动跨度(um)"),							//971
	_T("Reverse Angle (Deg)"),									_T("反向运动角度(度)"),							//972
	_T("Loop Height (um)"),										_T("线弧高度(um)"),								//973
	_T("Stick Detection"),										_T("不粘检测"),									//974
	_T("BH is not in the safe position"),						_T("焊头未处在安全位置"),						//975
	_T("Wait on/off loader finish..."),							_T("请等待上下料完成再清轨道..."),				//976
	_T("Wire Feed Error, Please Check Bond Quality!"),			_T("送线错误, 请检查焊线质量!"),				//977
	_T("Fast Tail"),											_T("快速线尾控制"),								//978
	_T("Wire Feed Check"),										_T("送线检测"),									//979
	_T("L Angle"),												_T("L偏转角度"),								//980
	_T("Index PR Delay (ms)"),									_T("夹具预夹紧延时(ms)"),						//981
	_T("Tail Length Monitor"),									_T("线尾长度监测"),								//982
	_T("Coarse Detection:"),									_T("松散检测:"),								//983
	_T("Coarse Detection Samples:"),							_T("松散检测样本:"),							//984
	_T("Matrix Die Ahead"),										_T("矩阵芯片预定位"),							//985
	_T("Matrix Row/Col"),										_T("矩阵行数/列数"),							//986	//v1.5.T91 edit
	_T("Move table to matrix 2nd-col top-right align pt"),		_T("移动工作台到矩阵第二列右上角定位点"),		//987
	_T("Move table to matrix 2nd-row top-right align pt"),		_T("移动工作台到矩阵第二行右上角定位点"),		//988
	_T("Unit Row/Col number wrong!"),							_T("单元行/列数错误!"),							//989
	_T("Matrix Row/Col number wrong!"),							_T("矩阵行/列数错误!"),							//990
	_T("Teach Matrix Pitch:"),									_T("指定矩阵间距:"),							//991
	_T("Power"),												_T("能量"),										//992
	_T("Time"),													_T("时间"),										//993
	_T("Impact Power (%)"),										_T("冲击能量(%)"),								//994
	_T("Bond Power (%)"),										_T("焊接能量(%)"),								//995
	_T("Post Damp Power (%)"),									_T("后置能量(%)"),								//996	//V1.1.W149 edit
	_T("Teach Pad Size"),										_T("指定焊盘尺寸"),								//997
	_T("Step1:"),												_T("步骤1:"),									//998
	_T("Move cross hair to top-left point of pad"),				_T("移动光标到焊盘的左上角"),					//999
	_T("Press Enter to continue"),								_T("按'确认'继续"),								//1000
	_T("Press ESC to cancel"),									_T("按'退出'取消"),								//1001
	_T("Esc"),													_T("取消"),										//1002
	_T("Enter"),												_T("确认"),										//1003
	_T("Step2:"),												_T("步骤2:"),									//1004
	_T("Step3:"),												_T("步骤3:"),									//1005
	_T("Move cross hair to top-right point of pad"),			_T("移动光标到焊盘的右上角"),					//1006
	_T("Move cross hair to bottom-right point of pad"),			_T("移动光标到焊盘的右下角"),					//1007
	_T("Edit Pad Size"),										_T("编辑焊盘尺寸"),								//1008
	_T("Step4:"),												_T("步骤4:"),									//1009
	_T("Teach Bond Points"),									_T("学习焊点位置"),								//1010
	_T("Move to group 1st point"),								_T("移动光标到小组的第一个点"),					//1011
	_T("Move to group 2nd point"),								_T("移动光标到小组的第二个点"),					//1012
	_T("Move to group last point"),								_T("移动光标到小组的最后一个点"),				//1013
	_T("Wire Length Control"),									_T("线长控制"),									//1014
	_T("Constant Wire Length"),									_T("固定线长"),									//1015
	_T("Wire Length"),											_T("线长"),										//1016
	_T("0: No; 1: Yes"),										_T("0:无; 1:有"),								//1017
	_T("Length:"),												_T("支架长："),									//1018
	_T("Confirm no LF between track and elevators"),			_T("请确认轨道与升降台之间无支架"),				//1019	//V1.1.W136 edit
	_T("Confirm no magazine under elevators"),					_T("请确认升降台下方无障碍"),					//1020	//V1.1.W136 edit
	_T("Pad Pattern Manager"),									_T("焊盘模板管理"),								//1021
	_T("Pad Patterns"),											_T("焊盘模板"),									//1022
	_T("Current"),												_T("当前"),										//1023
	_T("UPH Mode"),												_T("UPH模式"),									//1024
	_T("Ref"),													_T("定位"),										//1025
	_T("Pad Type"),												_T("焊盘类型"),									//1026
	_T("Teach Bond Point"),										_T("指定焊点"),									//1027
	_T("Taught(1/2)"),											_T("已教(1/2)"),								//1028
	_T("Single"),												_T("单点"),										//1029
	_T("Group"),												_T("组"),										//1030
	_T("Intelli Pad Locator"),									_T("智能焊盘定位"),								//1031
	_T("Bond On Property"),										_T("焊点属性"),									//1032
	_T("1st Bond Point Pad ID"),								_T("一焊点焊盘类型"),							//1033
	_T("1st Bond Point X Offset"),								_T("一焊点X偏移"),								//1034
	_T("1st Bond Point Y Offset"),								_T("一焊点Y偏移"),								//1035
	_T("Pad locating fail!"),									_T("焊盘定位失败!"),							//1036
	_T("Please manually locate pad"),							_T("请手动定位"),								//1037
	_T("Pad locating fail! Pls select pad ID."),				_T("焊盘定位失败!"),							//1038
	_T("Then manually locate pad."),							_T("请选择焊盘类型手动定位."),					//1039
	_T("1st and 2nd bond point number not match"),				_T("一焊二焊点数量不匹配"),						//1040
	_T("Capillary"),											_T("更换瓷咀"),									//1041
	_T("Change Capillary"),										_T("更换瓷咀"),									//1042
	_T("Remove old capillary"),									_T("拆除旧瓷咀"),								//1043
	_T("Insert new capillary"),									_T("安装新瓷咀"),								//1044
	_T("Insert capillary jig under transducer"),				_T("将瓷咀规安装在换能器下方"),					//1045
	_T("Tighten capillary with proper torque"),					_T("用正确的扭力锁紧瓷咀"),						//1046
	_T("Remove capillary jig"),									_T("拆除瓷咀规"),								//1047
	_T("Please check USG impedance"),							_T("请检查超声阻抗"),							//1048
	_T("Step5:"),												_T("步骤5:"),									//1049
	_T("Step6:"),												_T("步骤6:"),									//1050
	_T("Step7:"),												_T("步骤7:"),									//1051
	_T("Power On"),												_T("上电"),										//1052
	_T("Power Off"),											_T("断电"),										//1053
	_T("Teach Dummy Bond Area"),								_T("学习做尾区域"),								//1054
	_T("Move cross hair to top-left point"),					_T("移动光标到左上角"),							//1055
	_T("Move cross hair to bottom-right point"),				_T("移动光标到右下角"),							//1056
	_T("Press Continue to learn contact level"),				_T("按'继续'学习接触面高度"),					//1057
	_T("Teach"),												_T("学习"),										//1058
	_T("Bond Area"),											_T("做尾位置"),									//1059
	_T("Area Teach Status"),									_T("做尾位置学习状态"),							//1060
	_T("Swap Wire"),											_T("线序调换"),									//1061
	_T("First Wire"),											_T("第一条线"),									//1062
	_T("Second Wire"),											_T("第二条线"),									//1063
	_T("Wire Length Correction"),								_T("线长修正"),									//1064
	_T("1st Bond Parameter"),									_T("一焊参数"),									//1065
	_T("2nd Bond Parameter"),									_T("二焊参数"),									//1066
	_T("IPL Delay (ms)"),										_T("IPL延时 (ms)"),								//1067
	_T("Pre Kink"),												_T("预折点"),									//1068
	_T("Pre Kink Height (um)"),									_T("预折点高度 (um)"),							//1069
	_T("Pre Kink Dist (um)"),									_T("预折点长度 (um)"),							//1070
	_T("Pre Kink Angle (Deg)"),									_T("预折点角度 (度)"),							//1071
	_T("1st Pre-heat Delay (ms)"),								_T("首片预加热 (ms)"),							//1072
	_T("EFO Test"),												_T("打火测试"),									//1073	//v1.5.T71 edit
	_T(""),														_T(""),											//1074
	_T("Please convert to Single Unit first!"),					_T("请先转换为单一单元!"),						//1075
	_T("Single Unit"),											_T("单一单元"),									//1076
	_T("Group Wire"),											_T("焊线分组"),									//1077
	_T("Apply To:"),											_T("应用于:"),									//1078
	_T("Vacuum Control"),										_T("真空控制"),									//1079
	_T("Vacuum Pressure Too Low!"),								_T("真空气压过低!"),							//1080
	_T("Vacuum:"),												_T("真空:"),									//1081
	_T("Please select Program Type"),							_T("请选择程序类型"),							//1082	//v1.5.T91 edit
	_T("2nd Bond Point Pad ID"),								_T("二焊点焊盘类型"),							//1083
	_T("2nd Bond Point X Offset"),								_T("二焊点X偏移"),								//1084
	_T("2nd Bond Point Y Offset"),								_T("二焊点Y偏移"),								//1085
	_T("1st Bond Scrub"),										_T("一焊研磨"),									//1086
	_T("2nd Bond Scrub"),										_T("二焊研磨"),									//1087
	_T("Bump Ball Scrub"),										_T("垫球研磨"),									//1088
	_T("If need to change program,"),							_T("如果要修改程序,"),							//1089
	_T("quit this menu, then edit Manual and PR Align."),		_T("请从当前菜单退出,编辑手动和自动定位点。"),	//1090
	_T("If no need to change program,"),						_T("如果不修改程序,"),							//1091
	_T("X Motor overheat"),										_T("X马达过热"),								//1092
	_T("Y Motor overheat"),										_T("Y马达过热"),								//1093
	_T("EFO"),													_T("EFO"),										//1094
	_T("Spark Configuration"),									_T("打火参数"),									//1095
	_T("Gap Large DAC"),										_T("打火距离DAC"),								//1096
	_T("Length"),												_T("线长"),										//1097
	_T("CD"),													_T("CD"),										//1098
	_T("Profile"),												_T("弧形"),										//1099
	_T("Neck"),													_T("线颈"),										//1100
	_T("Kink2"),												_T("拐点2"),									//1101	//v3.1.T320 edit
	_T("Kink3"),												_T("拐点3"),									//1102	//v3.1.T320 edit
	_T("Twist"),												_T("扭弯"),										//1103
	_T("Landing"),												_T("着陆"),										//1104
	_T("Kink3 Span Length"),									_T("拐点3跨度"),								//1105	//v3.1.T320 edit
	_T("Kink3 Span Length Unit"),								_T("拐点3跨度单位"),							//1106	//v3.1.T320 edit
	_T("Kink3 Height Factor (%)"),								_T("拐点3高度比例 (%)"),						//1107	//v3.1.T320 edit
	_T("Kink3 Smoothness"),										_T("Kink3 Smoothness"),							//1108
	_T("Slope Length (um)"),									_T("Slope Length (um)"),						//1109
	_T("Contact Angle Correction (Degree)"),					_T("Contact Angle Correction (Degree)"),		//1110
	_T("Twist Ratio (%)"),										_T("Twist Ratio (%)"),							//1111
	_T("Are you sure to change loop profile?"),					_T("确定要改换线弧形状吗？"),					//1112
	_T("Are you sure to change loop type?"),					_T("确定要改换线弧类型吗？"),					//1113
	_T("Neck Wire Feed Speed (%)"),								_T("线颈出线速度 (%)"),							//1114
	_T("Neck Wire Bent Speed (%)"),								_T("线颈弯线速度 (%)"),							//1115
	_T("Pre Kink Wire Feed Speed (%)"),							_T("预折点出线速度 (%)"),						//1116
	_T("Pre Kink Wire Bent Speed (%)"),							_T("预折点弯线速度 (%)"),						//1117
	_T("Kink2 Wire Feed Speed (%)"),							_T("拐点2出线速度 (%)"),						//1118	//v3.1.T320 edit
	_T("Kink2 Wire Bent Speed (%)"),							_T("拐点2弯线速度 (%)"),						//1119	//v3.1.T320 edit
	_T("Kink3 Wire Feed Speed (%)"),							_T("拐点3出线速度 (%)"),						//1120	//v3.1.T320 edit
	_T("Kink3 Wire Bent Speed (%)"),							_T("拐点3弯线速度 (%)"),						//1121	//v3.1.T320 edit
	_T("Twist Wire Feed Speed (%)"),							_T("扭弯出线速度 (%)"),							//1122
	_T("Twist Reverse Bent Speed (%)"),							_T("扭弯反向弯线速度 (%)"),						//1123
	_T("Twist Forward Bent Speed (%)"),							_T("扭弯正向弯线速度 (%)"),						//1124
	_T("Trajectory Speed (%)"),									_T("线弧轨迹速度 (%)"),							//1125
	_T("Pull Speed (%)"),										_T("拉伸速度 (%)"),								//1126
	_T("Kink3 Height Pre-Move (%)"),							_T("拐点3高度提前移动 (%)"),					//1127	//v3.1.T320 edit
	_T("Kink3 Distance Pre-Move (%)"),							_T("拐点3距离提前移动 (%)"),					//1128	//v3.1.T320 edit
	_T("Reverse Height 4 Pre-Move (%)"),						_T("反向高度4提前移动 (%)"),					//1129
	_T("Twist Pre-Move (%)"),									_T("扭弯提前移动 (%)"),							//1130
	_T("Remove Magazines ON the Elevator"),						_T("请移除升降台上料盒！"),						//1131
	_T("Remove Magazines BELOW the Elevator"),					_T("请移除升降台下料盒！"),						//1132
	_T("Lead Tip Offset (um)"),									_T("引脚顶部偏移(um)"),							//1133
	_T("ILL Delay (ms)"),										_T("ILL延时(ms)"),								//1134
	_T("Lead Capillary Offset (um)"),							_T("引脚瓷咀偏移(um)"),							//1135
	_T("Intelli Lead Locator"),									_T("智能引脚定位"),								//1136
	_T("1st Bond Point ILL"),									_T("一焊点ILL"),								//1137
	_T("1st Bond Point Lead Capillary Offset"),					_T("一焊点引脚瓷咀偏移"),						//1138
	_T("1st Bond Point Lead X Offset"),							_T("一焊点引脚X偏移"),							//1139
	_T("1st Bond Point Lead Y Offset"),							_T("一焊点引脚Y偏移"),							//1140
	_T("2nd Bond Point ILL"),									_T("二焊点ILL"),								//1141
	_T("2nd Bond Point Lead Capillary Offset"),					_T("二焊点引脚瓷咀偏移"),						//1142
	_T("2nd Bond Point Lead X Offset"),							_T("二焊点引脚X偏移"),							//1143
	_T("2nd Bond Point Lead Y Offset"),							_T("二焊点引脚Y偏移"),							//1144
	_T("BSOB Cut Power (Dac)"),									_T("垫球切线能量 (Dac)"),						//1145
	_T("BSOB Cut Power Delay (%)"),								_T("垫球切线能量延时 (%)"),						//1146
	_T("Matrix Hybrid"),										_T("矩阵合成"),									//1147
	_T("Lead locating fail!"),									_T("引脚定位失败!"),							//1148
	_T("Please manually locate lead"),							_T("请手动定位引脚"),							//1149
	_T("Auto"),													_T("自动"),										//1150
	_T("Spark Open!"),											_T("打火失败！"),								//1151
	_T("Spark Gap Large!"),										_T("打火间距过大！"),							//1152
	_T("1st Unit Heat Delay(ms)"),								_T("首单元加热延时(ms)"),						//1153
	_T("Calibration In Progress..."),							_T("校准进行中..."),							//1154
	_T("USG Calibration"),										_T("超声波校准"),								//1155
	_T("Stepper Control"),										_T("步进马达控制"),								//1156
	_T("Capillary Auto Unplug"),								_T("自动通瓷咀"),								//1157
	_T("Unplug"),												_T("通瓷咀"),									//1158
	_T("Please Teach Dummy Bond Area!"),						_T("请先学习做尾区域!"),						//1159
	_T("Capillary Unplug Failure!"),							_T("通瓷咀失败!"),								//1160
	_T("Thread Wire"),											_T("穿线"),										//1161
	_T("Lead Frame Holder:"),									_T("支架保持器:"),								//1162
	_T("Matrix Information"),									_T("矩阵信息"),									//1163
	_T("Matrix Row:"),											_T("矩阵行数:"),								//1164
	_T("Matrix Column:"),										_T("矩阵列数:"),								//1165
	_T("Current Matrix:"),										_T("当前矩阵:"),								//1166
	_T("Matrix Bonding Schedule"),								_T("矩阵焊线计划"),								//1167
	_T("Matrix Layout"),										_T("矩阵发布"),									//1168
	_T("Index Time"),											_T("跳格次数"),									//1169
	_T("Total Row:"),											_T("总行数"),									//1170
	_T("Total Column:"),										_T("总列数"),									//1171
	_T("Current Row"),											_T("当前行"),									//1172
	_T("Current Column"),										_T("当前列"),									//1173
	_T("Reschedule"),											_T("重排计划"),									//1174
	_T("Width:"),												_T("支架宽度:"),								//1175
	_T("Mag Width:"),											_T("料盒宽度:"),								//1176
	_T("Mag Length:"),											_T("料盒长度:"),								//1177
	_T("Device Offset"),										_T("设备参数"),									//1178
	_T("Fork Detected on OnLoader"),							_T("上料料叉在! "),								//1179
	_T("Magazine Drop Detected on Offloader"),					_T("下料料盒在!"),								//1180
	_T("Magazine Drop Detected on Onloader"),					_T("上料料盒在!"),								//1181
	_T("Magazine Clamper"),										_T("料盒夹具"),									//1182
	_T("Material Handler"),										_T("过料系统"),									//1183
	_T("Clamp Pos"),											_T("夹紧位置"),									//1184
	_T("Onload Magazine Buffer Empty"),							_T("上料料盒已空"),								//1185
	_T("Offload Magazine Buffer Empty"),						_T("下料料盒已空"),								//1186
	_T("Offload Output Jam"),									_T("下料料盒卡住"),								//1187
	_T("Narrow"),												_T("窄"),										//1188
	_T("Wide"),													_T("宽"),										//1189
	_T("Please Check Rear Rail Type"),							_T("请检查后导轨类型"),							//1190
	_T("Wire Feed Count"),										_T("送线线数"),									//1191
	_T("Fork Detected on Offloader"),							_T("下料料叉在!"),								//1192
	_T("BSOB Offset"),											_T("垫球单线参数"),								//1193
	_T("BSOB Pre Impact Force 1"),								_T("垫球预备压力一"),							//1194
	_T("BSOB Pre Impact Time 1"),								_T("垫球预备时间一"),							//1195
	_T("BSOB Pre Impact Power 1"),								_T("垫球预备能量一"),							//1196
	_T("BSOB Time 1"),											_T("垫球时间一"),								//1197
	_T("BSOB Time 2"),											_T("垫球时间二"),								//1198
	_T("BSOB Power 1"),											_T("垫球能量一"),								//1199
	_T("BSOB Power 2"),											_T("垫球能量二"),								//1200
	_T("BSOB Force 1"),											_T("垫球压力一"),								//1201
	_T("BSOB Force 2"),											_T("垫球压力二"),								//1202
	_T("BSOB Impact Time 1"),									_T("垫球冲击时间一"),							//1203
	_T("BSOB Impact Time 2"),									_T("垫球冲击时间二"),							//1204
	_T("BSOB Impact Power 1"),									_T("垫球冲击能量一"),							//1205
	_T("BSOB Impact Power 2"),									_T("垫球冲击能量二"),							//1206
	_T("BSOB Impact Force 1"),									_T("垫球冲击压力一"),							//1207
	_T("BSOB Impact Force 2"),									_T("垫球冲击压力二"),							//1208
	_T("1st Bump Parameter"),									_T("一焊参数"),									//1209
	_T("2nd Bump Parameter"),									_T("二焊参数"),									//1210
	_T("Wire Length Exceed 8mm"),								_T("线长超过 8mm"),								//1211
	_T("Col"),													_T("列"),										//1212
	_T("Ref1"),													_T("定位一"),									//1213
	_T("Ref2"),													_T("定位二"),									//1214
	_T("Bump Shape Parameter"),									_T("垫球球形参数"),								//1215
	_T("Light"),												_T("光线"),										//1216
	_T("Please check Onload Y1 Sensor Plate"),					_T("检测 上料后夹马达传感器板"),				//1217 
	_T("Please check Onload Y2 Sensor Plate"),					_T("检测 上料前夹马达传感器板"),				//1218 
	_T("Please check Offload Y1 Sensor Plate"),					_T("检测 下料后马达传感器板"),					//1219 
	_T("Please check Offload Y2 Sensor Plate"),					_T("检测 下料前夹马达传感器板"),				//1220 
	_T("Please check Front Rail Sensor Plate"),					_T("检测 前导轨马达传传感器板"),				//1221 
	_T("Please check Rear Rail Sensor Plate"),					_T("检测 后导轨马达传感器板"),					//1222 
	_T("Please check lead frame at Onload "),					_T("检测 Onload lead frame"),					//1223
	_T("Motor Out of limt, cannot be moved "),					_T("马达超出工作范围"),							//1224 
	_T("Side Clamper"),											_T("侧夹具"),									//1225 
	_T("Front Clamper"),										_T("前夹具"),									//1226 
	_T("Rear Clamper"),											_T("后夹具"),									//1227 
	_T("Acc Ratio:"),											_T("加速比值:"),								//1228 
	_T("Front Rail"),											_T("前导轨"),									//1229
	_T("Rear Rail "),											_T("后导轨"),									//1230
	_T("Current Depth"),										_T("当前深度:"),								//1231
	_T("Ejector"),												_T("出料抓片"),									//1232
	_T("Move Distance"),										_T("移动距离"),									//1233
	_T("Distance to Home"),										_T("零位传感器距离"),							//1234
	_T("Front & Rear Clamper"),									_T("前后夹具"),									//1235
	_T("Injector Problem"),										_T("进料器失败"),								//1236
	_T("Shift"),												_T("平移"),										//1237
	_T("Width"),												_T("宽度"),										//1238
	_T("Please check Window Clamper Sensor Plate"),				_T("检测 夹具马达传感器板"),					//1239
	_T("Please close other control dialog first"),				_T("请关闭其他控制马达窗口"),					//1240
	_T("Save"),													_T("保存"),										//1241
	_T("Machine Offset"),										_T("机器参数"),									//1242
	_T("Inject"),												_T("推进单元"),									//1243
	_T("Search"),												_T("搜索单元"),									//1244
	_T("Compensate"),											_T("补偿单元"),									//1245
	_T("Catch Up"),												_T("追赶单元"),									//1246	
	_T("half Index"),											_T("半索引单元"),								//1247
	_T("Slot"),													_T("料盒总槽数:"),								//1248
	_T("Yes"),													_T("同意"),										//1249
	_T("No"),													_T("不同意"),									//1250
	_T("Height"),												_T("高度"),										//1251  
	_T("Self-define "),											_T("自定义"),									//1252
	_T("Cannot load from ID_CUELITE.ini "),						_T("不可以从 ID_CUELITE.ini 上载"),				//1253  
	_T("Upload by calculation ? "),								_T("使用计算上载 ?"),							//1254  
	_T("Save Machine Offset to ID_CUELITE.ini"),				_T("保存机器参数在 ID_CUELITE.ini"),			//1255
	_T("Injector"),												_T("进料抓片"),									//1256
	_T("Check Forks"),											_T("检查料叉"),									//1257
	_T("Value is negative, init carefully"),					_T("负数值,请小心检查,不要初始化 "),			//1258
	_T("Logic Zero to Home"),									_T("抓片器逻辑零点"),							//1259
	_T("Logic Right Limit to Home"),							_T("抓片器逻辑右极限"),							//1260
	_T("Standby Length (um)"),									_T("进料等待位置(um)"),							//1261
	_T("Right Clearance (um)"),									_T("右极限阈值(um)"),							//1262
	_T("Front & Rear Rail"),									_T("前后导轨"),									//1263
	_T("Please Manually Remove Window Clamper !"),				_T("请手动移除支架夹具！"),						//1264
	_T("Please Manually Install Window Clamper !"),				_T("请手动安装支架夹具！"),						//1265
	_T("Please Manually Remove Heater Block !"),				_T("请手动移除加热夹具！"),						//1266
	_T("Please Manually Install Heater Block !"),				_T("请手动安装加热夹具！"),						//1267
	_T("Cannot find machine offset reference file !"),			_T("机器偏移量参照文件不在！"),					//1268
	_T("Input Elevator Home fail !"),							_T("上料升降台马达不可回归起点！"),				//1269
	_T("Input Side Clamper Home fail !"),						_T("上料侧夹具马达不可回归起点！"),				//1270    
	_T("Input Rear Clamper Home fail !"),						_T("上料后夹具马达不可回归起点！"),				//1271
	_T("Input Front Clamper Home fail !"),						_T("上料前夹具马达不可回归起点！"),				//1272
	_T("Output Elevator Home fail !"),							_T("下料升降台马达不可回归起点！"),				//1273
	_T("Output Side Clamper Home fail !"),						_T("下料侧夹具马达不可回归起点！"),				//1274    
	_T("Output Rear Clamper Home fail !"),						_T("下料后夹具马达不可回归起点！"),				//1275
	_T("Output Front Clamper Home fail !"),						_T("下料前夹具马达不可回归起点！"),				//1276
	_T("Front Rail Home fail !"),								_T("前导轨马达不可回归起点！"),					//1277
	_T("Rear Rail Home fail !"),								_T("后导轨马达不可回归起点！"),					//1278    
	_T("Indexer Home fail !"),									_T("抓片器马达不可回归起点！"),					//1279
	_T("Window Clamper Home fail !"),							_T("支架夹具马达不可回归起点！"),				//1280
	_T("Wait Event Timeout!"),									_T("等待事件超时!"),							//1281
	_T("Rail Touch with Heater Block"),							_T("导轨碰到加热器!"),							//1282
	_T("Heat Protect (mins)"),									_T("过热热保护时间(分钟)"),						//1283
	_T("Load Material Handler Data?"),							_T("是否加载过料系统数据？"),					//1284	//V1.1.W172 edit
	_T("FAB Type"),												_T("FAB 组别"),									//1285
	_T("Is Bond Head at Safety Location ?"),					_T("请检查焊头在安全位置"),						//1286
	_T("1st Only"),												_T("只第一单元"),								//1287	//V1.4.W35 edit
	_T("AGC"),													_T("自动增益"),									//1288
	_T("Force,Auto"),											_T("压力,自动"),								//1289
	_T("Press 'Yes' to proceed, press 'No' to cancel"),			_T("按'同意'继续，按'不同意'取消"),				//1290	//V1.1.W250 edit
	_T("Unit"),													_T("单元"),										//1291
	_T("Indexer fail !"),										_T("抓片器马达移动失败！"),						//1292
	_T("Edge to Index Hole:"),									_T("索引孔与边沿距离"),							//1293
	_T("New Reel"),												_T("新支架卷"),									//1294 
	_T("Onload down!"),											_T("上料器停了!"),								//1295
	_T("Offload down!"),										_T("下料器停了!"),								//1296
	_T("Skip 1st "),											_T("跳过第一单元"),								//1297
	_T("Vacuum Check"),											_T("真空检查"),									//1298
	_T("Index time > 16 "),										_T("跳格次数 > 16"),							//1299
	_T("Onload ERROR!"),										_T("上料器出错!"),								//1300
	_T("Offload ERROR!"),										_T("下料器出错!"),								//1301
	_T("Onloader Mag Pully Jam"),								_T("上料拉料盒卡住"),							//1302
	_T("Offloader Mag Pully Jam"),								_T("下料拉料盒卡住"),							//1303
	_T("Clamp Pos"),											_T("夹持位置"),									//1304
	_T("Emergency Pressed!"),									_T("急停按下！"),								//1305
	_T("No LF Pushed Out!"),									_T("沒支架推出！"),								//1306
	_T("Elevator Calibration"),									_T("提升机校准"),								//1307
	_T("Next Mag Hold Pos:"),									_T("料盒夹紧位置:"),							//1308
	_T("Clamper Front Pos:"),									_T("料盒夹具前位置:"),							//1309
	_T("Clamper Back Pos:"),									_T("料盒夹具后位置:"),							//1310
	_T("Clamper Length Pos:"),									_T("料盒夹具长位置:"),							//1311
	_T("Front Track Pos:"),										_T("前导轨位置:"),								//1312
	_T("Back Track Pos:"),										_T("后导轨位置:"),								//1313
	_T("Clamper Len Limit1:"),									_T("料盒夹具长限位1:"),							//1314
	_T("Clamper Len Limit2:"),									_T("料盒夹具长限位2:"),							//1315
	_T("Clamper Len Limit3:"),									_T("料盒夹具长限位3:"),							//1316
	_T("Search Onloader MagLen Limit Fail!"),					_T("搜索上料料盒夹具长限位失败！"),				//1317
	_T("Search Onffloader MagLen Limit Fail!"),					_T("搜索下料料盒夹具长限位失败！"),				//1318
	_T("Center Offset:"),										_T("中心偏置:"),								//1319
	_T("Whether Load Bond Parameter?"),							_T("是否加载焊线参数？"),						//1320
	_T("Whether Load Material Parameter?"),						_T("是否加载物料参数？"),						//1321
	_T("Period of validity: "),									_T("有效期天数："),								//1322
	_T("Please contact the supplier to unlock!"),				_T("请联系设备供应商解锁！"),					//1323
	_T("Unlock Success!"),										_T("解锁成功！"),								//1324
	_T("Unlock Fail!"),											_T("解锁失败！"),								//1325
	_T("Unlock"),												_T("解锁"),										//1326
	_T("Period of validity: "),									_T("有效期："),									//1327
	_T("Open Track"),											_T("打开导轨"),									//1328
	_T("Close Track"),											_T("关闭导轨"),									//1329
	_T("Unplug?"),												_T("是否要通瓷咀？"),							//1330
	_T("Onloader Total Slot"),									_T("上料料盒总层数"),							//1331
	_T("Offloader Total Slot"),									_T("下料料盒总层数"),							//1332
	_T("Auto Skip Score:"),										_T("自动跳过分数："),							//1333
	_T("Skipped the whole Unit, Contiune?"),					_T("跳过了整个单元，是否继续？"),				//1334
	_T("PR Fail!"),												_T("图像匹配失败！"),							//1335
	_T("PR Quality Reject/ Out of range!"),						_T("图像图像质量差/超限！"),					//1336
	_T("PR Index Score"),										_T("进料图像分数"),								//1337
	_T("Manual Skip"),											_T("手动跳过"),									//1338
	_T("Fast Loop"),											_T("快速弧顶控制"),								//1339
	_T("Offload"),												_T("下料"),										//1340
	_T("LF Kick Out Pos(X):"),									_T("踢片器卸片位置(X):"),						//1341
	_T("Mag Support Pos(Y&&Z):"),								_T("料盒支架位置(Y和Z):"),						//1342
	_T("Mag Drop Pos(Z):"),										_T("料盒提取位置(Z):"),							//1343
	_T("First LF Pos(Y&&Z):"),									_T("第一片支架位置(Y和Z):"),					//1344
	_T("Mag Offload Pos(Y&&Z):"),								_T("料盒卸载位置(Y和Z):"),						//1345
	_T("Mag Pitch && Slot No.:"),								_T("料盒列间距和列数:"),						//1346
	_T("LF Kicker"),											_T("支架踢片器"),								//1347
	_T("Mag Indexer Y"),										_T("料盒抓片器Y"),								//1348
	_T("Mag Indexer Z"),										_T("料盒抓片器Z"),								//1349
	_T("Mag Kicker"),											_T("料盒踢片器"),								//1350
	_T("Mag Arm Empty"),										_T("料盒盒移动手已空"),							//1351
	_T("Mag Drop Failed"),										_T("下料料盒落下失败"),							//1352
	_T("Mag Kicker Full"),										_T("下料料盒踢片器已满"),						//1353
	_T("Mag Arm Full"),											_T("料盒盒移动手已满"),							//1354
	_T("Load Mag"),												_T("下载料盒"),									//1355
	_T("Pitch +"),												_T("移前料盒"),									//1356
	_T("Pitch -"),												_T("移后料盒"),									//1357
	_T("LF Output Jam"),										_T("料盒支架卡住!"),							//1358
	_T("Eject Mag"),											_T("踢出料盒"),									//1359
	_T("Bond Force Limit:"),									_T("焊接压力极限"),								//1360
	_T("Pre Impact Force 1"),									_T("冲击预备压力一"),							//1361
	_T("Pre Impact Force 2"),									_T("冲击预备压力二"),							//1362
	_T("Pre Impact Time 1"),									_T("冲击预备时间一"),							//1363
	_T("Pre Impact Time 2"),									_T("冲击预备时间二"),							//1364
	_T("Select Wire:"),											_T("选择连线："),								//1365
	_T("Backup PR 1"),											_T("备份模板一"),								//1366
	_T("Backup PR 2"),											_T("备份模板二"),								//1367
	_T("Backup PR 3"),											_T("备份模板三"),								//1368
	_T("Backup PR 4"),											_T("备份模板四"),								//1369
	_T("1st Bond Point"),										_T("第一焊点"),									//1370
	_T("2nd Bond Point"),										_T("第二焊点"),									//1371
	_T("Remove the Top Plate and Window Clamper"),				_T("请移除加热板和支架夹具"),					//1372
	_T("Have the Top Plate and Window Clamper been Removed?"),	_T("确定已经移除加热板和支架夹具？"),			//1373
	_T("Be careful on Capillary! Unplug?"),						_T("注意瓷咀部分！ 通瓷咀?"),					//1374
	_T("Search Range Retry:"),									_T("重试搜索范围："),							//1375
	_T("Relay Safety Duration:"),								_T("继电器安全期限："),							//1376
	_T("Output Mag Slot Sequence:"),							_T("下料料盒槽顺序:"),							//1377	//V1.4.W35 edit
	_T("Standard"),												_T("标准顺序"),									//1378
	_T("Alternate"),											_T("隔槽顺序"),									//1379
	_T("Invalid file name (exceed 31 characters)"),				_T("文件名不能超过31个字符"),					//1380
	_T("Spark Unknown Error!"),									_T("打火未知错误！"),							//1381
	_T("Spark Missing Result!"),								_T("打火结果丢失！"),							//1382
	_T("Next Step Pusher In:"),									_T("下一步推进："),								//1383
	_T("Close"),												_T("退出"),										//1384
	_T("Step 3:"),												_T("第三步:"),									//1385
	_T("Are you sure to save and overwrite current device setting?"),	_T("确定保存更改材料参数？"),			//1386
	_T("PreClamp Ht Close Delay (ms):"),						_T("夹持高度关闭延时:"),						//1387
	_T("Are you sure ready to Pre Clamp Height?"),				_T("确定开到预加紧位置？"),						//1388
	_T("Twist Forward Height Offset"),							_T("Twist Forward Height Offset"),				//1389
	_T("Auto Pad Centering:"),									_T("自动焊盘居中："),							//1390
	_T("Please Remove Elevator Support Bar"),					_T("请移除升降台支撑支架"),						//1391
	_T("Gap Fill:"),											_T("缺口填充："),								//1392
	_T("Binary:"),												_T("二值化："),									//1393
	_T("Centering Type:"),										_T("居中类型："),								//1394
	_T("Gravity"),												_T("重心"),										//1395
	_T("Circle"),												_T("圆心"),										//1396
	_T("Bond Head Time Offset"),								_T("焊头偏移时间"),								//1397
	_T("Full Conversion"),										_T("全部转换"),									//1398
	_T("Partial Conversion"),									_T("部分转换"),									//1399
	_T("Blob+Edge"),											_T("对象块+边界"),								//1400
	_T("Edge+Blob"),											_T("边界+对象块"),								//1401
	_T("Blob"),													_T("对象块"),									//1402
	_T("Edge"),													_T("边界"),										//1403
	_T("PR Overlap!"),											_T("图像匹配交叠"),								//1404
	_T("Die Overlap"),											_T("芯片交叠"),									//1405
	_T("Heat Delay Unit:"),										_T("加热延时单位："),							//1406
	_T("msec"),													_T("毫秒"),										//1407
	_T("sec"),													_T("秒"),										//1408
	_T("Next LF Wait Pos (mm):"),								_T("后续支架等待位置 (mm)："),					//1409
	_T("Rework"),												_T("返工"),										//1410
	_T("Press 'Confirm' to exit, press 'Stop' to cancel"),		_T("按'确认'退出，按'停止'取消"),				//1411
	_T("User Level"),											_T("用户级别"),									//1412	//V1.1.W38 add
	_T("Minimize Window"),										_T("最小化窗口"),								//1413	//V1.1.W51 add
	_T("Press 'Confirm' to update, press 'Stop' to cancel"),	_T("按'确认'更新，按'停止'取消"),				//1414	//V1.1.W141 add
	_T("USG Output Level"),										_T("超声输出模式"),								//1415	//V1.1.W149 add
	_T("Elevator Side"),										_T("升降台"),									//1416	//V1.1.W149 add
	_T("Clamp Distance"),										_T("夹紧距离"),									//1417	//V1.1.W149 add
	_T("Drive Speed:"),											_T("夹紧速度:"),								//1418	//V1.1.W149 add
	_T("Vacuum Delay:"),										_T("真空延时:"),								//1419	//V1.1.W149 add
	_T("Skip Die:"),											_T("跳过芯片:"),								//1420	//V1.1.W250 add
	_T("Skip Unit:"),											_T("跳过单元:"),								//1421	//V1.1.W250 add
	_T("Statistics of 'Skip Die/Unit' will be cleared"),		_T("'跳过芯片/单元'的统计将被清零"),			//1422	//V1.1.W250 add
	_T("Wire Length equal to zero! Wire Number: %d"),			_T("线长等于零！ 线号: %d"),					//1423	//V1.1.W254 add
	_T("MC#"),													_T("机号#"),									//1424	//V1.1.W258 add
	_T("Warning On Skip Num Per Index"),						_T("每次进料跳过警告数目"),						//1425	//V1.1.W262 add
	_T("'Skip Die/Unit Num Per Index' exceeds limit"),			_T("'每次进料跳过的芯片/单元数量'超过顶限"),	//1426	//V1.1.W262 add
	_T("'Serial Skip Die/Unit Num' exceeds limit"),				_T("'连续跳过的芯片/单元数量'超过顶限"),		//1427	//V1.1.W265 add
	_T("Flatten LF before PR Index"),							_T("进料图像前压平支架"),						//1428	//V1.4.W35 add
	_T("LF Flattening Duration(ms)"),							_T("支架压平持续时间(ms)"),						//1429	//V1.4.W35 add
	_T("Exceed Power 1 Range!"),								_T("超出能量一范围！"),							//1430	//v1.5.T4 add
	_T("Exceed Power 2 Range!"),								_T("超出能量二范围！"),							//1431	//v1.5.T7 add
	_T("Window Clamper Not Closed Properly"),					_T("支架夹具没有关紧"),							//1432	//V1.5.W11 add
	_T("Window Clamper Not Opened Properly"),					_T("支架夹具没有打开"),							//1433	//V1.5.W11 add
	_T("Exceed Force 1 Range!"),								_T("超出压力一范围！"),							//1434
	_T("Exceed Force 2 Range!"),								_T("超出压力二范围！"),							//1435
	_T("Exceed Time 1 Range!"),									_T("超出时间一范围！"),							//1436
	_T("Exceed Time 2 Range!"),									_T("超出时间二范围！"),							//1437
	_T("Total Cycle"),											_T("总周期数"),									//1438	//v1.5.T71 add
	_T("Bond Process Configure2"),								_T("焊接过程配置二"),							//1439	//v1.6.T126 add
	_T("Slow search failed!"),									_T("慢搜索失败!"),								//1440	//v1.6.P31 add
	_T("Reset Current Value to Default"),						_T("重置当前值"),								//1441	//v1.6.P196 add
	_T("Load Config"),											_T("加载设备配置"),								//1442	//v1.6.P238 add
	_T("Time 2"),												_T("时间二"),									//1443
	_T("Time 3"),												_T("时间三"),									//1444
	_T("Current 1"),											_T("电流一"),									//1445
	_T("Current 2"),											_T("电流二"),									//1446
	_T("Current 3"),											_T("电流三"),									//1447
	_T("Time 1"),												_T("时间一"),									//1448
	_T("Verify Recipe"),										_T("验证程式"),									//1449
	_T("1st bond contact low"),									_T("一焊接触高度过低"),							//1450
	_T("1st bond contact high"),								_T("一焊接触高度过高"),							//1451
	_T("2nd bond contact low"),									_T("二焊接触高度过低"),							//1452
	_T("2nd bond contact high"),								_T("二焊接触高度过高"),							//1453
	_T("Search Height 1"),										_T("搜索高度一"),								//1454	//v3.0.T2 add
	_T("Search Height 2"),										_T("搜索高度二"),								//1455	//v3.0.T2 add
	_T("Load Machine Config..."),								_T("加载设备配置..."),							//1456	//v3.0.T137 add
	_T("Init Device Parameter..."),								_T("初始化材料参数..."),						//1457	//v3.0.T137 add
	_T("Init Stepper..."),										_T("初始化步进电机..."),						//1458	//v3.0.T137 add
	_T("Init Servo..."),										_T("初始化伺服电机..."),						//1459	//v3.0.T137 add
	_T("Init Machine Parameter..."),							_T("初始化设备参数..."),						//1460	//v3.0.T137 add
	_T("Check software exit normally or not..."),				_T("检查软件是否正常退出..."),					//1461	//v3.0.T137 add
	_T("Init CAN Channel 1..."),								_T("初始化CAN总线通道一..."),					//1462	//v3.0.T137 add
	_T("Init USG..."),											_T("初始化超声箱..."),							//1463	//v3.0.T137 add
	_T("Initializing..."),										_T("初始化..."),								//1464	//v3.0.T137 add
	_T("Init Lighting Comm..."),								_T("初始化灯光通讯..."),						//1465	//v3.0.T137 add
	_T("Init Vision System..."),								_T("初始化图像系统..."),						//1466	//v3.0.T137 add
	_T("Load Power-On Parameter..."),							_T("加载开机参数..."),							//1467	//v3.0.T137 add
	_T("Init Bonder..."),										_T("初始化设备..."),							//1468	//v3.0.T137 add
	_T("Init EFO..."),											_T("初始化打火箱..."),							//1469	//v3.0.T137 add
	_T("Init Wire Clamp..."),									_T("初始化打火箱..."),							//1470	//v3.0.T137 add
	_T("Init Material Handler..."),								_T("初始化过料系统..."),						//1471	//v3.0.T137 add
	_T("Init XY Table..."),										_T("初始化XY工作台..."),						//1472	//v3.0.T137 add
	_T("Load PR Pattern..."),									_T("加载图像模板..."),							//1473	//v3.0.T137 add
	_T("This Chip Is Reverse!"),								_T("该芯片的位置颠倒"),							//1474	//v3.0.L284 add
	_T("Move table to left 2nd unit point"),					_T("移动工作台到左二单元定位点"),				//1475	//v3.0.T311 add
	_T("Move table to down 2nd unit point"),					_T("移动工作台到下二单元定位点"),				//1476	//v3.0.T311 add
	//v3.0.T433 add
	_T("BBOS Offset"),											_T("压球单线参数"),								//1477
	_T("BBOS Base Height (um)"),								_T("压球球基高度 (um)"),						//1478
	_T("BBOS Span Length (um)"),								_T("压球斜跨长度 (um)"),						//1479
	_T("BBOS Ball Thickness (um)"),								_T("压球球形厚度 (um)"),						//1480
	_T("BBOS Scrub Distance (um)"),								_T("压球刮研距离 (um)"),						//1481
	_T("BBOS Cut Power (Dac)"),									_T("压球切线能量 (Dac)"),						//1482
	_T("BBOS Cut Power Delay (%)"),								_T("压球切线能量延时 (%)"),						//1483
	_T("BBOS Motion Speed (%)"),								_T("压球运动速度 (%)"),							//1484
	_T("BBOS Direction Angle (Deg)"),							_T("压球方向角度 (度)"),						//1485
	//v3.0.T434 add
	_T("BBOS Pre Impact Time 1"),								_T("压球预备时间一"),							//1486
	_T("BBOS Pre Impact Power 1"),								_T("压球预备能量一"),							//1487
	_T("BBOS Pre Impact Force 1"),								_T("压球预备压力一"),							//1488
	_T("BBOS Impact Time 1"),									_T("压球冲击时间一"),							//1489
	_T("BBOS Impact Time 2"),									_T("压球冲击时间二"),							//1490
	_T("BBOS Impact Power 1"),									_T("压球冲击能量一"),							//1491
	_T("BBOS Impact Power 2"),									_T("压球冲击能量二"),							//1492
	_T("BBOS Impact Force 1"),									_T("压球冲击压力一"),							//1493
	_T("BBOS Impact Force 2"),									_T("压球冲击压力二"),							//1494
	_T("BBOS Time 1"),											_T("压球时间一"),								//1495
	_T("BBOS Time 2"),											_T("压球时间二"),								//1496
	_T("BBOS Power 1"),											_T("压球能量一"),								//1497
	_T("BBOS Power 2"),											_T("压球能量二"),								//1498
	_T("BBOS Force 1"),											_T("压球压力一"),								//1499
	_T("BBOS Force 2"),											_T("压球压力二"),								//1500
	_T("Auto Search"),											_T("自动搜索"),									//1501
	_T("Select 'Operation', then press 'Start'"),				_T("选择'操作', 然后按'开始'"),					//1502
	_T("- Install indexer alignment jig"),						_T("- 安装抓片器校准工具"),						//1503
	_T("- Press arrow keys until jig is aligned at"),			_T("- 按上下键直至校准工具"),					//1504
	_T("  center mark of bond site"),							_T("  和焊区中心对齐"),							//1505
	_T("Press 'Up/Down' to adjust capillary level"),			_T("按'上下'调整瓷咀高度"),						//1506	//v3.0.T454 add
	_T("Press 'Edit' to fine tune slot position"),				_T("按'编辑'微调槽间距"),						//1507
	_T("Above Loop"),											_T("顶弧"),										//1508	//v3.1.T288 add
	_T("Fast Loop"),											_T("快速"),										//1509	//v3.1.T288 add
	_T("BBOS Bump Shape Mode"),									_T("压球球形模式"),								//1510	//v3.1.T362 add
	_T("BBOS FAB Type"),										_T("压球 FAB 组别"),							//1511	//v3.1.T381 add
	_T("Bump Wire Parameter"),									_T("垫球焊线参数"),								//1512	//v3.1.T401 add
	_T("BSOB Wire Pre Impact Time"),							_T("垫球焊线预备时间"),							//1513	//v3.1.T401 add
	_T("BSOB Wire Pre Impact Force"),							_T("垫球焊线预备压力"),							//1514	//v3.1.T401 add
	_T("BSOB Wire Pre Impact Power"),							_T("垫球焊线预备能量"),							//1515	//v3.1.T401 add
	_T("BSOB Wire Impact Time"),								_T("垫球焊线冲击时间"),							//1516	//v3.1.T401 add
	_T("BSOB Wire Impact Force"),								_T("垫球焊线冲击压力"),							//1517	//v3.1.T401 add
	_T("BSOB Wire Impact Power"),								_T("垫球焊线冲击能量"),							//1518	//v3.1.T401 add
	_T("BSOB Wire Bond Time"),									_T("垫球焊线焊接时间"),							//1519	//v3.1.T401 add
	_T("BSOB Wire Bond Force"),									_T("垫球焊线焊接压力"),							//1520	//v3.1.T401 add
	_T("BSOB Wire Bond Power"),									_T("垫球焊线焊接能量"),							//1521	//v3.1.T401 add
	_T("BSOB Wire Post Damp Time"),								_T("垫球焊线后置时间"),							//1522	//v3.1.T401 add
	_T("BSOB Wire Post Damp Force"),							_T("垫球焊线后置压力"),							//1523	//v3.1.T401 add
	_T("BSOB Wire Post Damp Power"),							_T("垫球焊线后置能量"),							//1524	//v3.1.T401 add
	_T("Position Offset"),										_T("位置偏移"),									//1525	//v3.1.T410 add
	_T("Span Length Delay"),									_T("跨度延时"),									//1526	//v3.1.T410 add
	_T("Arc Span"),												_T("弧跨度"),									//1527	//v3.1.T410 add
	_T("Arc Span Delay"),										_T("弧跨度延时"),								//1528	//v3.1.T410 add
	_T("Touchdown Force (gm)"),									_T("探底压力 (gm)"),							//1529	//v3.1.T432 add
	_T("Touchdown Range (%)"),									_T("探底压力范围 (%)"),							//1530	//v3.1.T432 add
	_T("Touchdown Time (ms)"),									_T("探底时间 (ms)"),							//1531	//v3.1.T432 add
	_T("Gap Large Time (10us)"),								_T("打火距离时间(10us)"),						//1532	//v3.1.T484 add

	_T("Message Out Range"),									_T("信息溢出"),
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
		//要把窗口置顶，只要把show出来的Dlg的资源属性中的Topmost设置成true即可
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
