// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <string>

// Local
#include "Forward.h"
#include "Geometry.h"

namespace komp
{
// According to Behind Bars:
//      Beam thickness is 1/2 stave space. The distance
//      between beams is 1/4 stave space.  A fractional
//      beam is the width of a notehead.

// TODO: The DataSpace value should be more nuanced.
// See Behind Bars p. 42, Recommended distances before first note
// https://drive.google.com/open?id=0B9fjatRea7AaOFg2YmIwYmM2NEk

struct Spacing
{
    double StemLength{7};
    double BeamHeight{StemLength / 2};
    double CurveY{3};
    double ClefSpace{0.5};
    double KeySignatureSpace{0.5};
    double MeterSpace{0.5};
    // Space between the first symbol and the preceeding barline/clef/meter/key signature
    double DataSpace{1.0};
};


class Engraver
{
public:
    static Engraver& instance();
        
    double scaleFactor() const;
    
    void setMilimeters(double inValue);
    void setTenths(double inValue);
    
    double stemLineWidth() const;
    void setStemLineWidth(double inValue);
    
    double beamLineWidth() const;
    void setBeamLineWidth(double inValue);
    
    double staffLineWidth() const;
    void setStaffLineWidth(double inValue);
    
    double lightBarlineWidth() const;
    void setLightBarlineWidth(double inValue);
    
    double heavyBarlineWidth() const;
    void setHeavyBarlineWidth(double inValue);
    
    double endingLineWidth() const;
    void setEndingLineWidth(double inValue);
    
    double legerLineWidth() const;
    void setLegerLineWidth(double inValue);
    
    double wedgeLineWidth() const;
    void setWedgeLineWidth(double inValue);
    
    double enclosureLineWidth() const;
    void setEnclosureLineWidth(double inValue);
    
    double tupletBracketLineWidth() const;
    void setTupletBracketLineWidth(double inValue);
    
    double graceNoteSize() const;
    void setGraceNoteSize(double inValue);
    
    double cueNoteSize() const;
    void setCueNoteSize(double inValue);
    
    double hyphenDistance() const;
    void setHyphenDistance(double inValue);
    
    double beamDistance() const;
    void setBeamDistance(double inValue);
    
    double graceNoteScale() const;
    void setGraceNoteScale(double inValue);
    
    const Spacing& spacing() const;
    
private:
    Engraver() = default;

    double scaledValue(double inValue) const;
    
    double mMillimeters{7.0};
    double mTenths{40};
    double mStemLineWidth{1.275};
    double mBeamLineWidth{5.0391};
    double mStaffLineWidth{0.957};
    double mLightBarlineWidth{1.875};
    double mHeavyBarlineWidth{5.0391};
    double mEndingLineWidth{0.957};
    double mLegerLineWidth{1.875};
    double mWedgeLineWidth{0.957};
    double mEnclosureLineWidth{0.957};
    double mTupletBracketLineWidth{0.957};
    double mGraceNoteSize{60};
    double mCueNoteSize{60};
    double mHyphenDistance{120};
    double mBeamDistance{8};
    double mGraceNoteScale{0.6};
    
    Spacing mSpacing{};
};

inline void Engraver::setMilimeters(double inValue) { mMillimeters = inValue; }
inline void Engraver::setTenths(double inValue) { mTenths = inValue; }
inline void Engraver::setStemLineWidth(double inValue) { mStemLineWidth = inValue; }
inline void Engraver::setBeamLineWidth(double inValue) { mBeamLineWidth = inValue; }
inline void Engraver::setStaffLineWidth(double inValue) { mStaffLineWidth = inValue; }
inline void Engraver::setLightBarlineWidth(double inValue) { mLightBarlineWidth = inValue; }
inline void Engraver::setHeavyBarlineWidth(double inValue) { mHeavyBarlineWidth = inValue; }
inline void Engraver::setEndingLineWidth(double inValue) { mEndingLineWidth = inValue; }
inline void Engraver::setLegerLineWidth(double inValue) { mLegerLineWidth = inValue; }
inline void Engraver::setWedgeLineWidth(double inValue) { mWedgeLineWidth = inValue; }
inline void Engraver::setEnclosureLineWidth(double inValue) { mEnclosureLineWidth = inValue; }
inline void Engraver::setTupletBracketLineWidth(double inValue) { mTupletBracketLineWidth = inValue; }
inline void Engraver::setGraceNoteSize(double inValue) { mGraceNoteSize = inValue; }
inline void Engraver::setCueNoteSize(double inValue) { mCueNoteSize = inValue; }
inline void Engraver::setHyphenDistance(double inValue) { mHyphenDistance = inValue; }
inline void Engraver::setBeamDistance(double inValue) { mBeamDistance = inValue; }
inline void Engraver::setGraceNoteScale(double inValue) { mGraceNoteScale = inValue; }

inline const Spacing& Engraver::spacing() const { return mSpacing; }

} // namespace


// Text routines
double CharacterWidth(const double inFontSize, unsigned short inChar);
komp::Rect CharacterFrame(const double inFontSize, unsigned short inChar);
komp::Size MeasureCharacter(const double inFontSize, unsigned short inChar);
komp::Size  MeasureText(std::string_view inFontName, const double inFontSize, const char* inText);
double TextWidth(std::string_view inFontName, const double inFontSize, const char* inText);
double TextHeight(std::string_view inFontName, const double inFontSize, const char* inText);

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
