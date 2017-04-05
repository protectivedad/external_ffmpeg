#ifndef AVCODEC_BLURAY_TEXT_SUBTITLE_H
#define AVCODEC_BLURAY_TEXT_SUBTITLE_H

/*
 * Blu-ray Text subtitle decoder
 * Copyright (c) 2014 Fuzhou Rockchip Electronics Co., Ltd
 * hh@rock-chips.com
 */

#include "avcodec.h"

#define MAX_TEXTSUBTITLE_LINE 100
#define MAX_CHARS_PER_DPS_REGION 256

#define BD_TEXTST_FLOW_LEFT_RIGHT  1  /* Left-to-Right character progression, Top-to-Bottom line progression */
#define BD_TEXTST_FLOW_RIGHT_LEFT  2  /* Right-to-Left character progression, Top-to-Bottom line progression */
#define BD_TEXTST_FLOW_TOP_BOTTOM  3  /* Top-to-Bottom character progression, Right-to-Left line progression */

#define BD_TEXTST_HALIGN_LEFT   1
#define BD_TEXTST_HALIGN_CENTER 2
#define BD_TEXTST_HALIGN_RIGHT  3

#define BD_TEXTST_VALIGN_TOP    1
#define BD_TEXTST_VALIGN_MIDDLE 2
#define BD_TEXTST_VALIGN_BOTTOM 3

#define BD_TEXTST_FONT_OUTLINE_THIN   1
#define BD_TEXTST_FONT_OUTLINE_MEDIUM 2
#define BD_TEXTST_FONT_OUTLINE_THICK  3

#define BD_TEXTST_DATA_STRING      1
#define BD_TEXTST_DATA_FONT_ID     2
#define BD_TEXTST_DATA_FONT_STYLE  3
#define BD_TEXTST_DATA_FONT_SIZE   4
#define BD_TEXTST_DATA_FONT_COLOR  5
#define BD_TEXTST_DATA_NEWLINE     0x0a
#define BD_TEXTST_DATA_RESET_STYLE 0x0b

/**
 * TextST Constraints
 */
#define TEXTST_SPB_SIZE                 (512*1024)      // Subtitle Preloading Buffer Size = 512K
#define TEXTST_DB_SIZE                  (2*1024)        // Dialog Buffer Size = 2K
#define TEXTST_DCB_SIZE                 (32*1024)       // Dialog Composition Buffer Size = 32K
#define TEXTST_BOB_SIZE                 (2*1024*1024)   // Bitmap Object Buffer Size = 2M
#define TEXTST_BOB_DEPTH                10
#define TEXTST_MAX_DSS_REGION_STYLES    60
#define TEXTST_MAX_DSS_USER_STYLES      25
#define TEXTST_MAX_PALETTE_ENTRIES      255
#define TEXTST_MAX_REGIONS_PER_DPS      2
#define TEXTST_MAX_CHARS_PER_DPS_REGION 255
#define TEXTST_MAX_FONTS                255
#define TEXTST_MAX_FONTSIZE             144
#define TEXTST_MIN_FONTSIZE             8



enum SegmentType {
    /* Text subtitles */
    Text_DIALOG_STYLE        = 0x81,
    Text_DIALOG_PRESENTATION = 0x82,
};


typedef struct PaletteEntry
{
    unsigned int  color[256];
}PaletteEntry;

/*
* A "Text box" is defined by position and size within a Text Region. Text boxes are used for the
   placement of text for display
*/
typedef struct TextSubtitleRect
{
    int x,y;
    int mWidth,mHight;
}TextSubtitleRect;


/*
*   A "Text Region" is defined by position and size within the Graphic plane. Each Text Region can
    have a unique background color.
*/
typedef struct RectRegion
{
    TextSubtitleRect mRegion;
    int   mBackColorPaletteRefId; /* palette entry id ref */
}RectRegion;

typedef struct FontStyle
{
    int    mBold;
    int    mItalic;
    int    mOutLineBorder;
}FontStyle;

typedef struct RegionStyle
{
    unsigned char     mRegionStyleId;

    /*
    *  mTextFlow ���ֵ����з�ʽ  1(����): ���������з�ʽ    2: ������Ҫ��ת
    */
    unsigned char     mTextFlow;            /* BD_TEXTST_FLOW_* */

    /*
    *  mTextHalign  �ж��뷽ʽ   1: ����� 3: �Ҷ���  ����: ����
    */
    unsigned char     mTextHalign;          /* BD_TEXTST_HALIGN_* */ // horizontal_alignment

    /*
    * mTextValign  �ж��뷽ʽ   1: ���˶���(Top align)   2: ����   ����: �׶˶���(Bottom align)
    */
    unsigned char     mTextValign;          /* BD_TEXTST_VALIGN_* */ // vertical_alignment

    /*
    *  ÿһ��֮��ļ��
    */
    unsigned char     mLineSpace;

    /*
    * �����ʶ
    */
    unsigned char     mFontIdRef;

    unsigned char     mFontIncDec;  /* ���ݸñ�־λ���ж��Ƿ���Ե��������С�����ӻ��Ǽ�С */
    unsigned char     mFontSize;
    unsigned char     mFontColor;           /* palette entry id ref */
    unsigned char     mOutLineColor;        /* palette entry id ref */
    unsigned char     mOuntLIneThickness;    /* BD_TEXTST_FONT_OUTLINE_* */

    RectRegion        mRegion; 
    TextSubtitleRect  mTextRect;                  /* relative to region */
    FontStyle         mFontStyle;
}RegionStyle;

typedef struct UserStyle
{
    unsigned char  mUserStyleId;

    unsigned char  mFontIncDec;
    unsigned char  mFontSizeDelta;   // adjust Font size,increase/descrease this Delta

    unsigned char  mLineSpaceIncDec;  // 0 increase  1: decrease
    unsigned char  mLineSpaceDelta;

    unsigned char  mRegionHorizontalDiretion;
    unsigned char  mRegionVerticalDirection;

    unsigned char  mTextBoxHorizontalDirection;
    unsigned char  mTextBoxVerticalDirection;
    
    unsigned char  mTextBoxWidthIncDec;
    unsigned char  mTextBoxHeightIncDec;
    
    short          mRegionHorizontaDelta;
    short          mRegionVerticalDelta;
    short          mTextBoxHorizontalDelta;
    short          mTextBoxVerticalDelta;
    short          mTextBoxWidthDelta;
    short          mTextBoxHeightDelta;
}UserStyle;

typedef struct TextSubtitleText
{
    unsigned char mLength;
    unsigned char* mString;
}TextSubtitleText;

typedef struct DataStyle
{
    FontStyle          mFontStyle;
    unsigned char      mOutLineColor;
    unsigned char      mOutLineThickness;
}DataStyle;


/*
*  A Line Text Subtitle һ����Ļ����
*/ 
typedef struct TextLineData
{
    unsigned char*       mText;
    unsigned char*       mInLineStyleValues;
    unsigned char*       mInLineStyleType;
    unsigned char*       mInLineStylePosition;
}TextLineData;


typedef struct TextSubtitleData
{
    unsigned char mType;
    
    unsigned char mFontRefId;
    unsigned char mFontSize;
    unsigned char mFontColor;

    TextSubtitleText* mText;
    DataStyle* mDataStyle;
}TextSubtitleData;

typedef struct DialogRegion
{
    /* NOTE: this flag indicates a continuous presentation between this DPS and the NEXT DPS */
    unsigned char mContinousPresentFlag; 

    /*
     * If display is off, but the forced on flag is set, then
     * turn the display on.
     */
    unsigned char mForceOnFlag;
    unsigned char mRegionStyleRefId;

    // how many line in this Region
    int mLineCount;
    TextLineData* mLineData[MAX_TEXTSUBTITLE_LINE];
}DialogRegion;

typedef struct DialogStyle
{
    unsigned char   mPlayerStyleFlag;
    unsigned char   mRegoinStyleCount;
    unsigned char   mUserStyleCount;

    RegionStyle*    mRegionStyle;
    UserStyle*      mUserStyle;   // ��userStyle����ʱ������ʹ��userStyle���������壬�߿��
    PaletteEntry*   mPalette;
}DialogStyle;

typedef struct DialogPresentation
{
    int64_t        mStartPTS;
    int64_t        mEndPTS;

    int            mRegionCount;
    int            mPaletteUpdataFlag;
    PaletteEntry*  mPalette;
    DialogRegion*  mRegion;
}DialogPresentation;

typedef struct TextSubtitle
{
    DialogPresentation*  mDialog;
    DialogStyle*         mDialogStyle;
}TextSubtitle;
#endif
