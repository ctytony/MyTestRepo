/***************************************************************
VisionApp.h : implementation file
History:
28 Feb, 2011	--- Zhang Dong @1
- Support enable/disable vision image display.
15 Mar, 2011	--- Zhang Dong @2
- Support exposure time.
30 Mar, 2011	--- Zhao Bing @3
- Draw wire layout on overlay
11 April, 2011	--- Zhao Bing @4
- Draw Circle on overlay
04 Aug, 2011	--- Zhang Dong @5
- Support pattern mask feature.
- Support vision debug ModeB.
17 May, 2012	--- Zhang Dong @6
- Seperate image capture into exposure and image transfer.
05 Nov, 2012	--- Zhang Dong @7
- Support auto pad display.
21 Mar, 2013	--- Zhang Dong @8
- Support inteligent lead locate.
****************************************************************/

#pragma once

class CVisionApp
{
public:
    CVisionApp(void);
    ~CVisionApp(void);
    // Initialize vision system and display window
    int Initialize(short nClientNum, HWND hDispWnd1, HWND hDispWnd2 = NULL);

	int InitUsbCcd(short nClientNum, HWND hDispWnd1, HWND hDispWnd2 = NULL);

    // Free vision resource
    int FreeVisionResource(void);
    // Start live video of destinated client
    int StartLiveVideo(short nClientId);
    // Stop live video of destinated client
    int StopLiveVideo(short nClientId);
    // Capture image of destinated client
    int CaptureImage(short nClientId);
    // Draw cursor on overlay
    int DrawCursor(short nClientId);
    // Draw rectangle on overlay
    int DrawRectangle(short nClientId, short nWidth, short nHeight);
    // Draw Rect
	int DrawRect(short nClientId, int iStartX, int iStartY, int iEndX, int iEndY, short nColor, short nWireNum = 0); //@7
    // Clear overlay content
    int ClearOverlay(short nClientId);
    // Show overlay content
    int ShowOverlay(short nClientId, bool bEnable);
    // Binarize image
    int BinarizeImage(short nClientId, short nThreshold);
    // Learn pattern and return pattern ID
    int LearnPattern(short nClientId, bool bRotateEnable, short nPatternWidth, short nPatternHeight, long* plPatternId, bool bIndexPr);
    // Search pattern
    int SearchPattern(short nClientId, long lPatternId, short nSearchWidth, short nSearchHeight, SEARCH_PATTERN_RESULT* pstSearchResult, short nBackUpId);
    // Clear specific pattern record
    int ClearPatternRcd(short nClientId, long lPatternId);
    // Save power on data
    int SavePowerOnData(void);
    // Load power on data
    int LoadPowerOnData(void);
    // Save vision data to file
    int SaveVisionDataToFile(char* pchFileName);
    // Load vision data from file
    int LoadVisionDataFromFile(char* pchFileName);
    // Clear all pattern records of specific client
    int ClearAllPatternRcd(short nClientId, short bClearType);
    // Draw pattern on overlay
    int DrawPatternOnOverlay(short nClientId, long lPatternId);
    // Set pattern match pass score
    int SetPatternPassScore(short nClientId, long lPatternId, short nPassScore);	//v10.1
    // Set debug mode
    int SetVisionDebugMode(short nClientId, char chDebugEnable); //@5
    // Get pattern record
    int GetPatternRecord(short nClientId, short nPatternNum, PATTERN_RECORD *pstPatternRcd);
    // Get vision version
    int GetVersion(float *pfVisionVer);	//v10.1
    // Display image or not
    int DisplayImage(short nClientId, bool bIsEnable); //@1
    // Set camera exposure time
    int SetCameraExposureTime(short nClientId, short nExposureTime); //@2
	// Draw wire layout on overlay
	int UpdateWireLayout(short nClientId, int iStartX, int iStartY, int iEndX, int iEndY, short nWireNum, short nColor); //@3
	// Display bond point layout on the overlay
	int UpdateBondPtLayout(short nClientId, int iStartX, int iStartY, short nBondPt, short nWireNum, short nColor); //@7
    //Draw Circle
	int DrawCircle(short nClientId, long lXCenter, long lYCenter, long lRadius); //@4
    // Alloc mask buffer
    int AllocMaskBuffer(short nClientId, short nWidth, short nHeight); //@5
    // Get mask buffer valid flag
    bool GetMaskBufferValid(short nClientId);
    // Mask buffer image point
    int MaskBufferPoint(short nClientId, short nPixalX, short nPixalY);
    // Display mask on overlay
    int DisplayMask(short nClientId, short nPixalX, short nPixalY);
    // Set PR rotate angle range
    int SetRotateAngle(short nClientId, short nAngle);

	int SetImageParameter(short nClientId, BYTE iRedGain, int iContrast,int iBrightness,int iSaturation);
    int EnableImageProcess(short nClientId, short nEnable);
    // Wait image transfer complete
    int WaitImageTransfer(short nClientId); //@6
    // Binary image auto threshold
    int BinaryAutoThreshold(short nClientId, short* pnThreshold); //@8
    // Locate lead center
    int LocateLeadCenter(short nClientId, short nOrgX, short nOrgY, double dOrgAngle, 
                         short nThreshold, short nTipOffset, 
                         double* pdCenterX, double* pdCenterY);
};
