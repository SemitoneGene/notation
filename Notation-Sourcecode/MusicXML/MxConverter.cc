// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MxConverter.h"

// System
#include <cmath>

// Komp
#include "ArpeggiateType.h"
#include "Clef.h"
#include "Constants.h"
#include "Measure.h"
#include "Note.h"
#include "BeamChunk.h"
#include "Pitch.h"
#include "RoundPlaces.h"
#include "Score.h"
#include "Staff.h"
#include "StaffSystem.h"

// items local to the translation unit
namespace
{
static komp::MxMark emptyMark{
    mx::api::MarkType::unspecified,
    mx::api::Placement::unspecified,
    komp::MarkType::NONE};


template<typename KOMP, typename MX>
MX findMxItem(const std::map<const KOMP, const MX>& enumMap, MX defaultToReturn, KOMP itemToFind)
{
    auto it = enumMap.find( itemToFind );
    if (it == enumMap.cend() ) {
        return defaultToReturn;
    }
    return it->second;
}

// private local
template<typename KOMP, typename MX>
KOMP findKompItem(const std::map<const KOMP, const MX>& enumMap, KOMP defaultToReturn, MX itemToFind)
{
    auto compare = [&itemToFind]( const std::pair<KOMP, MX>& v ) {
        return v.second == itemToFind;
    };
    auto it = std::find_if(enumMap.cbegin(), enumMap.cend(), compare );
    if (it == enumMap.cend() ) {
        return defaultToReturn;
    }
    return it->first;
}

} // end anonymous namespace

namespace komp
{
MxConverterParams::MxConverterParams()
: tenths(1.0)
, millimeters(1.0)
, pixelsPerInch(PIXELS_PER_INCH)
{
    
}

MxConverterParams::MxConverterParams(Score& inScore)
: tenths(1.0)
, millimeters(1.0)
, pixelsPerInch(PIXELS_PER_INCH)
{
    KOMP_ASSERT(inScore.getStaffSystems().size() >= 1);
    const auto& system = inScore.getStaffSystems().front();
    KOMP_ASSERT(system);
    KOMP_ASSERT(system->getStaves().size() >= 1);
    const auto& staff = system->getStaves().front();
    KOMP_ASSERT(staff);
    const long double spaceSizePixels = Staff::getSpacing();
    const long double spaceSizeInches = spaceSizePixels / pixelsPerInch;
    const long double spaceSizeMillimeters = spaceSizeInches * MxConverter::MILLIMETERS_PER_INCH; // millimeters per inch
    
    millimeters = (spaceSizeMillimeters * 4.0);
    tenths = 40.0;
}

MxConverter::MxConverter(MxConverterParams params)
: mMxMillimeters(params.millimeters)
, mMxTenths(params.tenths)
, mPixelsPerInch(params.pixelsPerInch)
{
    
}

long double
MxConverter::convertTenthsToPixels(long double mxTenths) const
{
    return mxTenths * getPixelsPerTenth();
}

long double
MxConverter::convertPixelsToTenths(long double pixels) const
{
    return pixels / getPixelsPerTenth();
}

int
MxConverter::convertTenthsToRoundedPixels(long double mxTenths) const
{
    return static_cast<int>(std::ceil(convertTenthsToPixels(mxTenths) - 0.5f));
}

int
MxConverter::convertPixelsToRoundedTenths(long double pixels) const
{
    return static_cast<int>(std::ceil(convertPixelsToTenths(pixels) - 0.5f));
}

long double
MxConverter::getTenthsPerMillimeter() const
{
    return mMxTenths / mMxMillimeters;
}

long double
MxConverter::getTenthsPerInch() const
{
    return getTenthsPerMillimeter() * static_cast<long double>(MILLIMETERS_PER_INCH);
}

long double
MxConverter::getPixelsPerTenth() const
{
    return getPixelsPerInch() / getTenthsPerInch();
}

long double
MxConverter::getPixelsPerInch() const
{
    return mPixelsPerInch;
}

int
MxConverter::convertDefaultYToSlot(double inDefaultY) const
{
    // TODO: magin number assumes five-line staff
    // TODO: ignores possibility of unique staff size
    // TODO: assumes <tenths>40</tenths>
    double spaceDistance = inDefaultY / 10.0;
    double slotDistance = spaceDistance * 2.0;
    double slot = 10.0 + slotDistance;
    return static_cast<int>(std::ceil(slot - 0.5));
}

double
MxConverter::convertSlotToDefaultY(int inSlot) const
{
    // TODO: magin number assumes five-line staff
    // TODO: ignores possibility of unique staff size
    // TODO: assumes <tenths>40</tenths>
    double slotDistance = inSlot - 10;
    double spaceDistance = slotDistance / 2;
    double defaultY = komp::roundPlaces(spaceDistance * 10.0, 5);
    return defaultY;
}

const MxMarkSet MxConverter::sMarks =
{
    MxMark{mx::api::MarkType::unspecified, mx::api::Placement::above, komp::MarkType::NONE},
    MxMark{mx::api::MarkType::unspecified, mx::api::Placement::below, komp::MarkType::NONE},
    MxMark{mx::api::MarkType::unspecified, mx::api::Placement::unspecified, komp::MarkType::NONE},
    
    MxMark{mx::api::MarkType::accidentalMarkFlat, mx::api::Placement::above, komp::MarkType::NATURAL},
    MxMark{mx::api::MarkType::accidentalMarkFlat, mx::api::Placement::below, komp::MarkType::NATURAL},
    MxMark{mx::api::MarkType::accidentalMarkFlat, mx::api::Placement::unspecified, komp::MarkType::NATURAL},
    
    MxMark{mx::api::MarkType::accidentalMarkNatural, mx::api::Placement::above, komp::MarkType::NATURAL},
    MxMark{mx::api::MarkType::accidentalMarkNatural, mx::api::Placement::below, komp::MarkType::NATURAL},
    MxMark{mx::api::MarkType::accidentalMarkNatural, mx::api::Placement::unspecified, komp::MarkType::NATURAL},
    
    MxMark{mx::api::MarkType::accidentalMarkSharp, mx::api::Placement::above, komp::MarkType::SHARP},
    MxMark{mx::api::MarkType::accidentalMarkSharp, mx::api::Placement::below, komp::MarkType::SHARP},
    MxMark{mx::api::MarkType::accidentalMarkSharp, mx::api::Placement::unspecified, komp::MarkType::SHARP},
    
    MxMark{mx::api::MarkType::trillMark, mx::api::Placement::above, komp::MarkType::TRILL},
    MxMark{mx::api::MarkType::trillMark, mx::api::Placement::below, komp::MarkType::TRILL},
    MxMark{mx::api::MarkType::trillMark, mx::api::Placement::unspecified, komp::MarkType::TRILL},
    
    MxMark{mx::api::MarkType::turn, mx::api::Placement::above, komp::MarkType::TURN},
    MxMark{mx::api::MarkType::turn, mx::api::Placement::below, komp::MarkType::TURN},
    MxMark{mx::api::MarkType::turn, mx::api::Placement::unspecified, komp::MarkType::TURN},
    
    //        MxMark{mx::api::MarkType::slashTurn, mx::api::Placement::above, komp::MarkType::TURN_SLASH},
    //        MxMark{mx::api::MarkType::slashTurn, mx::api::Placement::below, komp::MarkType::TURN_SLASH},
    //        MxMark{mx::api::MarkType::slashTurn, mx::api::Placement::unspecified, komp::MarkType::TURN_SLASH},
    
    MxMark{mx::api::MarkType::invertedTurn, mx::api::Placement::above, komp::MarkType::TURN_INVERTED},
    MxMark{mx::api::MarkType::invertedTurn, mx::api::Placement::below, komp::MarkType::TURN_INVERTED},
    MxMark{mx::api::MarkType::invertedTurn, mx::api::Placement::unspecified, komp::MarkType::TURN_INVERTED},
    
    MxMark{mx::api::MarkType::mordent, mx::api::Placement::above, komp::MarkType::MORDENT},
    MxMark{mx::api::MarkType::mordent, mx::api::Placement::below, komp::MarkType::MORDENT},
    MxMark{mx::api::MarkType::mordent, mx::api::Placement::unspecified, komp::MarkType::MORDENT},
    
    MxMark{mx::api::MarkType::invertedMordent, mx::api::Placement::above, komp::MarkType::MORDENT_INVERTED},
    MxMark{mx::api::MarkType::invertedMordent, mx::api::Placement::below, komp::MarkType::MORDENT_INVERTED},
    MxMark{mx::api::MarkType::invertedMordent, mx::api::Placement::unspecified, komp::MarkType::MORDENT_INVERTED},
    
    MxMark{mx::api::MarkType::tenuto, mx::api::Placement::above, komp::MarkType::TENUTO_ABOVE},
    MxMark{mx::api::MarkType::tenuto, mx::api::Placement::below, komp::MarkType::TENUTO_BELOW},
    MxMark{mx::api::MarkType::tenuto, mx::api::Placement::unspecified, komp::MarkType::TENUTO_ABOVE},
    
    MxMark{mx::api::MarkType::detachedLegato, mx::api::Placement::above, komp::MarkType::TENUTO_STACCATO_ABOVE},
    MxMark{mx::api::MarkType::detachedLegato, mx::api::Placement::below, komp::MarkType::TENUTO_STACCATO_BELOW},
    MxMark{mx::api::MarkType::detachedLegato, mx::api::Placement::unspecified, komp::MarkType::TENUTO_STACCATO_ABOVE},
    
    MxMark{mx::api::MarkType::accent, mx::api::Placement::above, komp::MarkType::ACCENT_ABOVE},
    MxMark{mx::api::MarkType::accent, mx::api::Placement::below, komp::MarkType::ACCENT_BELOW},
    MxMark{mx::api::MarkType::accent, mx::api::Placement::unspecified, komp::MarkType::ACCENT_ABOVE},
    
    MxMark{mx::api::MarkType::otherArticulation, mx::api::Placement::above, komp::MarkType::ACCENT_STACCATO_ABOVE},
    MxMark{mx::api::MarkType::otherArticulation, mx::api::Placement::below, komp::MarkType::ACCENT_STACCATO_BELOW},
    MxMark{mx::api::MarkType::otherArticulation, mx::api::Placement::unspecified, komp::MarkType::ACCENT_STACCATO_ABOVE},
    
    MxMark{mx::api::MarkType::strongAccent, mx::api::Placement::above, komp::MarkType::MARCATO_ABOVE},
    MxMark{mx::api::MarkType::strongAccent, mx::api::Placement::below, komp::MarkType::MARCATO_BELOW},
    MxMark{mx::api::MarkType::strongAccent, mx::api::Placement::unspecified, komp::MarkType::MARCATO_ABOVE},
    
    MxMark{mx::api::MarkType::staccato, mx::api::Placement::above, komp::MarkType::STACCATO_ABOVE},
    MxMark{mx::api::MarkType::staccato, mx::api::Placement::below, komp::MarkType::STACCATO_BELOW},
    MxMark{mx::api::MarkType::staccato, mx::api::Placement::unspecified, komp::MarkType::STACCATO_ABOVE},
    
    MxMark{mx::api::MarkType::staccatissimo, mx::api::Placement::above, komp::MarkType::STACCATISSIMO_ABOVE},
    MxMark{mx::api::MarkType::staccatissimo, mx::api::Placement::below, komp::MarkType::STACCATISSIMO_BELOW},
    MxMark{mx::api::MarkType::staccatissimo, mx::api::Placement::unspecified, komp::MarkType::STACCATISSIMO_ABOVE},
    
    MxMark{mx::api::MarkType::otherArticulation, mx::api::Placement::above, komp::MarkType::MARCATO_STACCATO_ABOVE},
    MxMark{mx::api::MarkType::otherArticulation, mx::api::Placement::below, komp::MarkType::MARCATO_STACCATO_BELOW},
    MxMark{mx::api::MarkType::otherArticulation, mx::api::Placement::unspecified, komp::MarkType::MARCATO_STACCATO_ABOVE},
    
    MxMark{mx::api::MarkType::unspecified, mx::api::Placement::above, komp::MarkType::DOT},
    MxMark{mx::api::MarkType::unspecified, mx::api::Placement::below, komp::MarkType::DOT},
    MxMark{mx::api::MarkType::unspecified, mx::api::Placement::unspecified, komp::MarkType::DOT},
    
    MxMark{mx::api::MarkType::caesura, mx::api::Placement::above, komp::MarkType::CAESURA},
    MxMark{mx::api::MarkType::caesura, mx::api::Placement::below, komp::MarkType::CAESURA},
    MxMark{mx::api::MarkType::caesura, mx::api::Placement::unspecified, komp::MarkType::CAESURA},
    
    MxMark{mx::api::MarkType::downBow, mx::api::Placement::above, komp::MarkType::STRINGS_BOW_DOWN},
    MxMark{mx::api::MarkType::downBow, mx::api::Placement::below, komp::MarkType::STRINGS_BOW_DOWN},
    MxMark{mx::api::MarkType::downBow, mx::api::Placement::unspecified, komp::MarkType::STRINGS_BOW_DOWN},
    
    MxMark{mx::api::MarkType::upBow, mx::api::Placement::above, komp::MarkType::STRINGS_BOW_UP},
    MxMark{mx::api::MarkType::upBow, mx::api::Placement::below, komp::MarkType::STRINGS_BOW_UP},
    MxMark{mx::api::MarkType::upBow, mx::api::Placement::unspecified, komp::MarkType::STRINGS_BOW_UP},
    
    MxMark{mx::api::MarkType::harmonic, mx::api::Placement::above, komp::MarkType::STRINGS_HARMONIC},
    MxMark{mx::api::MarkType::harmonic, mx::api::Placement::below, komp::MarkType::STRINGS_HARMONIC},
    MxMark{mx::api::MarkType::harmonic, mx::api::Placement::unspecified, komp::MarkType::STRINGS_HARMONIC},
    
    MxMark{mx::api::MarkType::unspecified, mx::api::Placement::above, komp::MarkType::STRINGS_HARMONIC_HALF},
    MxMark{mx::api::MarkType::unspecified, mx::api::Placement::below, komp::MarkType::STRINGS_HARMONIC_HALF},
    MxMark{mx::api::MarkType::unspecified, mx::api::Placement::unspecified, komp::MarkType::STRINGS_HARMONIC_HALF},
    
    MxMark{mx::api::MarkType::tremoloSingleOne, mx::api::Placement::above, komp::MarkType::TREMOLO_ONE},
    MxMark{mx::api::MarkType::tremoloSingleOne, mx::api::Placement::below, komp::MarkType::TREMOLO_ONE},
    MxMark{mx::api::MarkType::tremoloSingleOne, mx::api::Placement::unspecified, komp::MarkType::TREMOLO_ONE},
    
    MxMark{mx::api::MarkType::tremoloSingleTwo, mx::api::Placement::above, komp::MarkType::TREMOLO_TWO},
    MxMark{mx::api::MarkType::tremoloSingleTwo, mx::api::Placement::below, komp::MarkType::TREMOLO_TWO},
    MxMark{mx::api::MarkType::tremoloSingleTwo, mx::api::Placement::unspecified, komp::MarkType::TREMOLO_TWO},
    
    MxMark{mx::api::MarkType::tremoloSingleThree, mx::api::Placement::above, komp::MarkType::TREMOLO_THREE},
    MxMark{mx::api::MarkType::tremoloSingleThree, mx::api::Placement::below, komp::MarkType::TREMOLO_THREE},
    MxMark{mx::api::MarkType::tremoloSingleThree, mx::api::Placement::unspecified, komp::MarkType::TREMOLO_THREE},
    
    MxMark{mx::api::MarkType::tremoloSingleFour, mx::api::Placement::above, komp::MarkType::TREMOLO_FOUR},
    MxMark{mx::api::MarkType::tremoloSingleFour, mx::api::Placement::below, komp::MarkType::TREMOLO_FOUR},
    MxMark{mx::api::MarkType::tremoloSingleFour, mx::api::Placement::unspecified, komp::MarkType::TREMOLO_FOUR},
    
    MxMark{mx::api::MarkType::tremoloSingleFive, mx::api::Placement::above, komp::MarkType::TREMOLO_FIVE},
    MxMark{mx::api::MarkType::tremoloSingleFive, mx::api::Placement::below, komp::MarkType::TREMOLO_FIVE},
    MxMark{mx::api::MarkType::tremoloSingleFive, mx::api::Placement::unspecified, komp::MarkType::TREMOLO_FIVE},
    
    MxMark{mx::api::MarkType::unspecified, mx::api::Placement::above, komp::MarkType::GRACE_SLASH_UP},
    MxMark{mx::api::MarkType::unspecified, mx::api::Placement::below, komp::MarkType::GRACE_SLASH_UP},
    MxMark{mx::api::MarkType::unspecified, mx::api::Placement::unspecified, komp::MarkType::GRACE_SLASH_UP},
    
    MxMark{mx::api::MarkType::unspecified, mx::api::Placement::above, komp::MarkType::GRACE_SLASH_DOWN},
    MxMark{mx::api::MarkType::unspecified, mx::api::Placement::below, komp::MarkType::GRACE_SLASH_DOWN},
    MxMark{mx::api::MarkType::unspecified, mx::api::Placement::unspecified, komp::MarkType::GRACE_SLASH_DOWN},
    
    MxMark{mx::api::MarkType::breathMark, mx::api::Placement::above, komp::MarkType::BREATH_MARK},
    MxMark{mx::api::MarkType::breathMark, mx::api::Placement::below, komp::MarkType::BREATH_MARK},
    MxMark{mx::api::MarkType::breathMark, mx::api::Placement::unspecified, komp::MarkType::BREATH_MARK},
    
    MxMark{mx::api::MarkType::doit, mx::api::Placement::above, komp::MarkType::BRASS_DOIT},
    MxMark{mx::api::MarkType::doit, mx::api::Placement::below, komp::MarkType::BRASS_DOIT},
    MxMark{mx::api::MarkType::doit, mx::api::Placement::unspecified, komp::MarkType::BRASS_DOIT},
    
    MxMark{mx::api::MarkType::unspecified, mx::api::Placement::above, komp::MarkType::BRASS_LIFT},
    MxMark{mx::api::MarkType::unspecified, mx::api::Placement::below, komp::MarkType::BRASS_LIFT},
    MxMark{mx::api::MarkType::unspecified, mx::api::Placement::unspecified, komp::MarkType::BRASS_LIFT},
    
    MxMark{mx::api::MarkType::falloff, mx::api::Placement::above, komp::MarkType::BRASS_FALLOFF},
    MxMark{mx::api::MarkType::falloff, mx::api::Placement::below, komp::MarkType::BRASS_FALLOFF},
    MxMark{mx::api::MarkType::falloff, mx::api::Placement::unspecified, komp::MarkType::BRASS_FALLOFF},
    
    MxMark{mx::api::MarkType::plop, mx::api::Placement::above, komp::MarkType::BRASS_PLOP},
    MxMark{mx::api::MarkType::plop, mx::api::Placement::below, komp::MarkType::BRASS_PLOP},
    MxMark{mx::api::MarkType::plop, mx::api::Placement::unspecified, komp::MarkType::BRASS_PLOP},
    
    MxMark{mx::api::MarkType::scoop, mx::api::Placement::above, komp::MarkType::BRASS_SCOOP},
    MxMark{mx::api::MarkType::scoop, mx::api::Placement::below, komp::MarkType::BRASS_SCOOP},
    MxMark{mx::api::MarkType::scoop, mx::api::Placement::unspecified, komp::MarkType::BRASS_SCOOP},
    
    MxMark{mx::api::MarkType::stress, mx::api::Placement::above, komp::MarkType::STRESS_ABOVE},
    MxMark{mx::api::MarkType::stress, mx::api::Placement::below, komp::MarkType::STRESS_BELOW},
    MxMark{mx::api::MarkType::stress, mx::api::Placement::unspecified, komp::MarkType::STRESS_ABOVE},
    
    MxMark{mx::api::MarkType::unstress, mx::api::Placement::above, komp::MarkType::UNSTRESS_ABOVE},
    MxMark{mx::api::MarkType::unstress, mx::api::Placement::below, komp::MarkType::UNSTRESS_BELOW},
    MxMark{mx::api::MarkType::unstress, mx::api::Placement::unspecified, komp::MarkType::UNSTRESS_ABOVE},
    
    MxMark{mx::api::MarkType::fermata, mx::api::Placement::above, komp::MarkType::FERMATA},
    MxMark{mx::api::MarkType::fermata, mx::api::Placement::below, komp::MarkType::FERMATA},
    MxMark{mx::api::MarkType::fermata, mx::api::Placement::unspecified, komp::MarkType::FERMATA},
    
    MxMark{mx::api::MarkType::fermataInverted, mx::api::Placement::above, komp::MarkType::FERMATA_INVERTED},
    MxMark{mx::api::MarkType::fermataInverted, mx::api::Placement::below, komp::MarkType::FERMATA_INVERTED},
    MxMark{mx::api::MarkType::fermataInverted, mx::api::Placement::unspecified, komp::MarkType::FERMATA_INVERTED},
    
    MxMark{mx::api::MarkType::fermataUpright, mx::api::Placement::above, komp::MarkType::FERMATA},
    MxMark{mx::api::MarkType::fermataUpright, mx::api::Placement::below, komp::MarkType::FERMATA},
    MxMark{mx::api::MarkType::fermataUpright, mx::api::Placement::unspecified, komp::MarkType::FERMATA},
    
    MxMark{mx::api::MarkType::fermataNormal, mx::api::Placement::above, komp::MarkType::FERMATA},
    MxMark{mx::api::MarkType::fermataNormal, mx::api::Placement::below, komp::MarkType::FERMATA},
    MxMark{mx::api::MarkType::fermataNormal, mx::api::Placement::unspecified, komp::MarkType::FERMATA},
    
    MxMark{mx::api::MarkType::fermataNormalInverted, mx::api::Placement::above, komp::MarkType::FERMATA_INVERTED},
    MxMark{mx::api::MarkType::fermataNormalInverted, mx::api::Placement::below, komp::MarkType::FERMATA_INVERTED},
    MxMark{mx::api::MarkType::fermataNormalInverted, mx::api::Placement::unspecified, komp::MarkType::FERMATA_INVERTED},
    
    MxMark{mx::api::MarkType::fermataNormalUpright, mx::api::Placement::above, komp::MarkType::FERMATA},
    MxMark{mx::api::MarkType::fermataNormalUpright, mx::api::Placement::below, komp::MarkType::FERMATA},
    MxMark{mx::api::MarkType::fermataNormalUpright, mx::api::Placement::unspecified, komp::MarkType::FERMATA},
    
    MxMark{mx::api::MarkType::fermataAngled, mx::api::Placement::above, komp::MarkType::FERMATA_SHORT_ABOVE},
    MxMark{mx::api::MarkType::fermataAngled, mx::api::Placement::below, komp::MarkType::FERMATA_SHORT_ABOVE},
    MxMark{mx::api::MarkType::fermataAngled, mx::api::Placement::unspecified, komp::MarkType::FERMATA_SHORT_ABOVE},
    
    MxMark{mx::api::MarkType::fermataAngledInverted, mx::api::Placement::above, komp::MarkType::FERMATA_SHORT_BELOW},
    MxMark{mx::api::MarkType::fermataAngledInverted, mx::api::Placement::below, komp::MarkType::FERMATA_SHORT_BELOW},
    MxMark{mx::api::MarkType::fermataAngledInverted, mx::api::Placement::unspecified, komp::MarkType::FERMATA_SHORT_BELOW},
    
    MxMark{mx::api::MarkType::fermataAngledUpright, mx::api::Placement::above, komp::MarkType::FERMATA_SHORT_ABOVE},
    MxMark{mx::api::MarkType::fermataAngledUpright, mx::api::Placement::below, komp::MarkType::FERMATA_SHORT_ABOVE},
    MxMark{mx::api::MarkType::fermataAngledUpright, mx::api::Placement::unspecified, komp::MarkType::FERMATA_SHORT_ABOVE},
    
    MxMark{mx::api::MarkType::fermataSquare, mx::api::Placement::above, komp::MarkType::FERMATA_LONG_ABOVE},
    MxMark{mx::api::MarkType::fermataSquare, mx::api::Placement::below, komp::MarkType::FERMATA_LONG_ABOVE},
    MxMark{mx::api::MarkType::fermataSquare, mx::api::Placement::unspecified, komp::MarkType::FERMATA_LONG_ABOVE},
    
    MxMark{mx::api::MarkType::fermataSquareInverted, mx::api::Placement::above, komp::MarkType::FERMATA_LONG_BELOW},
    MxMark{mx::api::MarkType::fermataSquareInverted, mx::api::Placement::below, komp::MarkType::FERMATA_LONG_BELOW},
    MxMark{mx::api::MarkType::fermataSquareInverted, mx::api::Placement::unspecified, komp::MarkType::FERMATA_LONG_BELOW},
    
    MxMark{mx::api::MarkType::fermataSquareUpright, mx::api::Placement::above, komp::MarkType::FERMATA_LONG_ABOVE},
    MxMark{mx::api::MarkType::fermataSquareUpright, mx::api::Placement::below, komp::MarkType::FERMATA_LONG_ABOVE},
    MxMark{mx::api::MarkType::fermataSquareUpright, mx::api::Placement::unspecified, komp::MarkType::FERMATA_LONG_ABOVE},
    
    MxMark{mx::api::MarkType::unknownDynamics, mx::api::Placement::above, komp::MarkType::DYNAMIC_UNKNOWN},
    MxMark{mx::api::MarkType::unknownDynamics, mx::api::Placement::below, komp::MarkType::DYNAMIC_UNKNOWN},
    MxMark{mx::api::MarkType::unknownDynamics, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_UNKNOWN},
    
    MxMark{mx::api::MarkType::otherDynamics, mx::api::Placement::above, komp::MarkType::DYNAMIC_OTHER},
    MxMark{mx::api::MarkType::otherDynamics, mx::api::Placement::below, komp::MarkType::DYNAMIC_OTHER},
    MxMark{mx::api::MarkType::otherDynamics, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_OTHER},
    
    MxMark{mx::api::MarkType::p, mx::api::Placement::above, komp::MarkType::DYNAMIC_P},
    MxMark{mx::api::MarkType::p, mx::api::Placement::below, komp::MarkType::DYNAMIC_P},
    MxMark{mx::api::MarkType::p, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_P},
    
    MxMark{mx::api::MarkType::pp, mx::api::Placement::above, komp::MarkType::DYNAMIC_PP},
    MxMark{mx::api::MarkType::pp, mx::api::Placement::below, komp::MarkType::DYNAMIC_PP},
    MxMark{mx::api::MarkType::pp, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_PP},
    
    MxMark{mx::api::MarkType::ppp, mx::api::Placement::above, komp::MarkType::DYNAMIC_PPP},
    MxMark{mx::api::MarkType::ppp, mx::api::Placement::below, komp::MarkType::DYNAMIC_PPP},
    MxMark{mx::api::MarkType::ppp, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_PPP},
    
    MxMark{mx::api::MarkType::pppp, mx::api::Placement::above, komp::MarkType::DYNAMIC_PPPP},
    MxMark{mx::api::MarkType::pppp, mx::api::Placement::below, komp::MarkType::DYNAMIC_PPPP},
    MxMark{mx::api::MarkType::pppp, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_PPPP},
    
    MxMark{mx::api::MarkType::ppppp, mx::api::Placement::above, komp::MarkType::DYNAMIC_PPPPP},
    MxMark{mx::api::MarkType::ppppp, mx::api::Placement::below, komp::MarkType::DYNAMIC_PPPPP},
    MxMark{mx::api::MarkType::ppppp, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_PPPPP},
    
    MxMark{mx::api::MarkType::pppppp, mx::api::Placement::above, komp::MarkType::DYNAMIC_PPPPPP},
    MxMark{mx::api::MarkType::pppppp, mx::api::Placement::below, komp::MarkType::DYNAMIC_PPPPPP},
    MxMark{mx::api::MarkType::pppppp, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_PPPPPP},
    
    MxMark{mx::api::MarkType::mp, mx::api::Placement::above, komp::MarkType::DYNAMIC_MP},
    MxMark{mx::api::MarkType::mp, mx::api::Placement::below, komp::MarkType::DYNAMIC_MP},
    MxMark{mx::api::MarkType::mp, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_MP},
    
    MxMark{mx::api::MarkType::f, mx::api::Placement::above, komp::MarkType::DYNAMIC_F},
    MxMark{mx::api::MarkType::f, mx::api::Placement::below, komp::MarkType::DYNAMIC_F},
    MxMark{mx::api::MarkType::f, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_F},
    
    MxMark{mx::api::MarkType::mf, mx::api::Placement::above, komp::MarkType::DYNAMIC_MF},
    MxMark{mx::api::MarkType::mf, mx::api::Placement::below, komp::MarkType::DYNAMIC_MF},
    MxMark{mx::api::MarkType::mf, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_MF},
    
    MxMark{mx::api::MarkType::ff, mx::api::Placement::above, komp::MarkType::DYNAMIC_FF},
    MxMark{mx::api::MarkType::ff, mx::api::Placement::below, komp::MarkType::DYNAMIC_FF},
    MxMark{mx::api::MarkType::ff, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_FF},
    
    MxMark{mx::api::MarkType::fff, mx::api::Placement::above, komp::MarkType::DYNAMIC_FFF},
    MxMark{mx::api::MarkType::fff, mx::api::Placement::below, komp::MarkType::DYNAMIC_FFF},
    MxMark{mx::api::MarkType::fff, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_FFF},
    
    MxMark{mx::api::MarkType::ffff, mx::api::Placement::above, komp::MarkType::DYNAMIC_FFFF},
    MxMark{mx::api::MarkType::ffff, mx::api::Placement::below, komp::MarkType::DYNAMIC_FFFF},
    MxMark{mx::api::MarkType::ffff, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_FFFF},
    
    MxMark{mx::api::MarkType::fffff, mx::api::Placement::above, komp::MarkType::DYNAMIC_FFFFF},
    MxMark{mx::api::MarkType::fffff, mx::api::Placement::below, komp::MarkType::DYNAMIC_FFFFF},
    MxMark{mx::api::MarkType::fffff, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_FFFFF},
    
    MxMark{mx::api::MarkType::ffffff, mx::api::Placement::above, komp::MarkType::DYNAMIC_FFFFFF},
    MxMark{mx::api::MarkType::ffffff, mx::api::Placement::below, komp::MarkType::DYNAMIC_FFFFFF},
    MxMark{mx::api::MarkType::ffffff, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_FFFFFF},
    
    // No Niente in MusicXML - must be handled elsewhere
    // MxMark{mx::api::MarkType::otherDynamics, mx::api::Placement::above, komp::MarkType::DYNAMIC_NIENTE},
    // MxMark{mx::api::MarkType::otherDynamics, mx::api::Placement::below, komp::MarkType::DYNAMIC_NIENTE},
    // MxMark{mx::api::MarkType::otherDynamics, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_NIENTE},
    
    MxMark{mx::api::MarkType::fp, mx::api::Placement::above, komp::MarkType::DYNAMIC_FP},
    MxMark{mx::api::MarkType::fp, mx::api::Placement::below, komp::MarkType::DYNAMIC_FP},
    MxMark{mx::api::MarkType::fp, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_FP},
    
    MxMark{mx::api::MarkType::fz, mx::api::Placement::above, komp::MarkType::DYNAMIC_FZ},
    MxMark{mx::api::MarkType::fz, mx::api::Placement::below, komp::MarkType::DYNAMIC_FZ},
    MxMark{mx::api::MarkType::fz, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_FZ},
    
    MxMark{mx::api::MarkType::rf, mx::api::Placement::above, komp::MarkType::DYNAMIC_RF},
    MxMark{mx::api::MarkType::rf, mx::api::Placement::below, komp::MarkType::DYNAMIC_RF},
    MxMark{mx::api::MarkType::rf, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_RF},
    
    MxMark{mx::api::MarkType::rfz, mx::api::Placement::above, komp::MarkType::DYNAMIC_RFZ},
    MxMark{mx::api::MarkType::rfz, mx::api::Placement::below, komp::MarkType::DYNAMIC_RFZ},
    MxMark{mx::api::MarkType::rfz, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_RFZ},
    
    // No 's' in MusicXML
    // MxMark{mx::api::MarkType::s, mx::api::Placement::above, komp::MarkType::DYNAMIC_S},
    // MxMark{mx::api::MarkType::s, mx::api::Placement::below, komp::MarkType::DYNAMIC_S},
    // MxMark{mx::api::MarkType::s, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_S},
    
    MxMark{mx::api::MarkType::sf, mx::api::Placement::above, komp::MarkType::DYNAMIC_SF},
    MxMark{mx::api::MarkType::sf, mx::api::Placement::below, komp::MarkType::DYNAMIC_SF},
    MxMark{mx::api::MarkType::sf, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_SF},
    
    MxMark{mx::api::MarkType::sffz, mx::api::Placement::above, komp::MarkType::DYNAMIC_SFFZ},
    MxMark{mx::api::MarkType::sffz, mx::api::Placement::below, komp::MarkType::DYNAMIC_SFFZ},
    MxMark{mx::api::MarkType::sffz, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_SFFZ},
    
    MxMark{mx::api::MarkType::sfp, mx::api::Placement::above, komp::MarkType::DYNAMIC_SFP},
    MxMark{mx::api::MarkType::sfp, mx::api::Placement::below, komp::MarkType::DYNAMIC_SFP},
    MxMark{mx::api::MarkType::sfp, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_SFP},
    
    MxMark{mx::api::MarkType::sfpp, mx::api::Placement::above, komp::MarkType::DYNAMIC_SFPP},
    MxMark{mx::api::MarkType::sfpp, mx::api::Placement::below, komp::MarkType::DYNAMIC_SFPP},
    MxMark{mx::api::MarkType::sfpp, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_SFPP},
    
    MxMark{mx::api::MarkType::sfz, mx::api::Placement::above, komp::MarkType::DYNAMIC_SFZ},
    MxMark{mx::api::MarkType::sfz, mx::api::Placement::below, komp::MarkType::DYNAMIC_SFZ},
    MxMark{mx::api::MarkType::sfz, mx::api::Placement::unspecified, komp::MarkType::DYNAMIC_SFZ},
    
};

const MxClefSymbolMap MxConverter::sClefs =
{
    std::make_pair(komp::ClefSymbolType::C, mx::api::ClefSymbol::c),
    std::make_pair(komp::ClefSymbolType::G, mx::api::ClefSymbol::g),
    std::make_pair(komp::ClefSymbolType::F, mx::api::ClefSymbol::f),
    std::make_pair(komp::ClefSymbolType::TAB, mx::api::ClefSymbol::tab),
    std::make_pair(komp::ClefSymbolType::PERCUSSION, mx::api::ClefSymbol::percussion),
    
    // unsupported by design
    // std::make_pair(komp::ClefSymbolType::UNKNOWN, mx::api::ClefSymbol::none),
    // std::make_pair(komp::ClefSymbolType::UNKNOWN, mx::api::ClefSymbol::jianpu),
};

const MxAccidental MxConverter::sAccidentals =
{
    std::make_pair(komp::AccidentalType::NONE, mx::api::Accidental::none),
    std::make_pair(komp::AccidentalType::SHARP, mx::api::Accidental::sharp),
    std::make_pair(komp::AccidentalType::FLAT, mx::api::Accidental::flat),
    std::make_pair(komp::AccidentalType::NATURAL, mx::api::Accidental::natural),
    std::make_pair(komp::AccidentalType::DOUBLE_SHARP, mx::api::Accidental::doubleSharp),
    std::make_pair(komp::AccidentalType::DOUBLE_FLAT, mx::api::Accidental::flatFlat),
    std::make_pair(komp::AccidentalType::SHARP_SHARP, mx::api::Accidental::sharpSharp),
    std::make_pair(komp::AccidentalType::NATURAL_SHARP, mx::api::Accidental::naturalSharp),
    std::make_pair(komp::AccidentalType::NATURAL_FLAT, mx::api::Accidental::naturalFlat),
    std::make_pair(komp::AccidentalType::QUARTER_FLAT, mx::api::Accidental::quarterFlat),
    std::make_pair(komp::AccidentalType::QUARTER_SHARP, mx::api::Accidental::quarterSharp),
    std::make_pair(komp::AccidentalType::TRIPLE_SHARP, mx::api::Accidental::tripleSharp),
    std::make_pair(komp::AccidentalType::TRIPLE_FLAT, mx::api::Accidental::tripleFlat),
    //std::make_pair(komp::AccidentalType::TOTAL_ACCIDENTALS, mx::api::Accidental::none),
};

const MxPitchStepMap MxConverter::sPitches =
{
    std::make_pair(komp::PitchStep::C, mx::api::Step::c),
    std::make_pair(komp::PitchStep::D, mx::api::Step::d),
    std::make_pair(komp::PitchStep::E, mx::api::Step::e),
    std::make_pair(komp::PitchStep::F, mx::api::Step::f),
    std::make_pair(komp::PitchStep::G, mx::api::Step::g),
    std::make_pair(komp::PitchStep::A, mx::api::Step::a),
    std::make_pair(komp::PitchStep::B, mx::api::Step::b),
};

const MxTemporalTypeMap MxConverter::sTemporalTypes =
{
    std::make_pair(TemporalType::WHOLE, mx::api::DurationName::whole),
    std::make_pair(TemporalType::HALF, mx::api::DurationName::half),
    std::make_pair(TemporalType::QUARTER, mx::api::DurationName::quarter),
    std::make_pair(TemporalType::EIGHTH, mx::api::DurationName::eighth),
    std::make_pair(TemporalType::SIXTEENTH, mx::api::DurationName::dur16th),
    std::make_pair(TemporalType::THIRTY_SECOND, mx::api::DurationName::dur32nd),
    std::make_pair(TemporalType::SIXTY_FOURTH, mx::api::DurationName::dur64th),
    std::make_pair(TemporalType::ONE_TWENTY_EIGHTH, mx::api::DurationName::dur128th),
    std::make_pair(TemporalType::TWO_FIFTY_SIXTH, mx::api::DurationName::dur256th),
    std::make_pair(TemporalType::FIVE_TWELFTH, mx::api::DurationName::dur512th),
    std::make_pair(TemporalType::TEN_TWENTY_FOURTH, mx::api::DurationName::dur1024th),
};

const MxNoteheadTypeMap MxConverter::sNoteheadTypes =
{
    std::make_pair(NoteheadType::SLASH_H_END, mx::api::Notehead::slash),
    std::make_pair(NoteheadType::TRIANGLE, mx::api::Notehead::triangle),
    std::make_pair(NoteheadType::DIAMOND, mx::api::Notehead::diamond),
    std::make_pair(NoteheadType::SQUARE, mx::api::Notehead::square),
    std::make_pair(NoteheadType::PLUS, mx::api::Notehead::cross),
    std::make_pair(NoteheadType::X, mx::api::Notehead::x),
    std::make_pair(NoteheadType::CIRCLED_X, mx::api::Notehead::circleX),
    std::make_pair(NoteheadType::TRIANGLE_INVERTED, mx::api::Notehead::invertedTriangle),
    std::make_pair(NoteheadType::UNMAPPED, mx::api::Notehead::arrowDown),
    std::make_pair(NoteheadType::UNMAPPED, mx::api::Notehead::arrowUp),
    std::make_pair(NoteheadType::UNMAPPED, mx::api::Notehead::slashed),
    std::make_pair(NoteheadType::UNMAPPED, mx::api::Notehead::backSlashed),
    std::make_pair(NoteheadType::NORMAL, mx::api::Notehead::normal),
    std::make_pair(NoteheadType::CLUSTER, mx::api::Notehead::cluster),
    std::make_pair(NoteheadType::UNMAPPED, mx::api::Notehead::circleDot),
    std::make_pair(NoteheadType::UNMAPPED, mx::api::Notehead::leftTriangle),
    std::make_pair(NoteheadType::UNMAPPED, mx::api::Notehead::rectangle),
    std::make_pair(NoteheadType::UNMAPPED, mx::api::Notehead::none),
    std::make_pair(NoteheadType::UNMAPPED, mx::api::Notehead::do_),
    std::make_pair(NoteheadType::UNMAPPED, mx::api::Notehead::re),
    std::make_pair(NoteheadType::UNMAPPED, mx::api::Notehead::mi),
    std::make_pair(NoteheadType::UNMAPPED, mx::api::Notehead::fa),
    std::make_pair(NoteheadType::UNMAPPED, mx::api::Notehead::la),
    std::make_pair(NoteheadType::UNMAPPED  , mx::api::Notehead::so),
    std::make_pair(NoteheadType::UNMAPPED, mx::api::Notehead::ti)
};

const MxTimeSignatureTypeMap MxConverter::sTimeSignatureTypes =
{
    std::make_pair(TimeSignatureType::COMMON, mx::api::TimeSignatureSymbol::common),
    std::make_pair(TimeSignatureType::CUT, mx::api::TimeSignatureSymbol::cut),
};

const MxOttavaMap MxConverter::sOttavaMap =
{
    std::make_pair(OctaveType::O_ALTA, mx::api::OttavaType::o8va),
    std::make_pair(OctaveType::O_BASSA_VB, mx::api::OttavaType::o8vb),
    std::make_pair(OctaveType::Q_ALTA, mx::api::OttavaType::o15ma),
    std::make_pair(OctaveType::Q_BASSA_MB, mx::api::OttavaType::o15mb),
};

const MxBeamChunkMap MxConverter::sBeamChunkMap =
{
    std::make_pair<BeamChunk, mx::api::Beam>(BeamChunk::Unknown, mx::api::Beam::unspecified),
    std::make_pair<BeamChunk, mx::api::Beam>(BeamChunk::Begin, mx::api::Beam::begin),
    std::make_pair<BeamChunk, mx::api::Beam>(BeamChunk::End, mx::api::Beam::end),
    std::make_pair<BeamChunk, mx::api::Beam>(BeamChunk::Continue, mx::api::Beam::extend),
    std::make_pair<BeamChunk, mx::api::Beam>(BeamChunk::BrokenLeft, mx::api::Beam::backwardBroken),
    std::make_pair<BeamChunk, mx::api::Beam>(BeamChunk::BrokenRight, mx::api::Beam::forwardBroken),
};

const MxSoundMap MxConverter::sSoundMap =
{
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_ALPHORN, mx::api::SoundID::brassAlphorn),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_ALTO_HORN, mx::api::SoundID::brassAltoHorn),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_BARITONE_HORN, mx::api::SoundID::brassBaritoneHorn),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_BUGLE, mx::api::SoundID::brassBugle),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_BUGLE_ALTO, mx::api::SoundID::brassBugleAlto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_BUGLE_BARITONE, mx::api::SoundID::brassBugleBaritone),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_BUGLE_CONTRABASS, mx::api::SoundID::brassBugleContrabass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_BUGLE_EUPHONIUM_BUGLE, mx::api::SoundID::brassBugleEuphoniumBugle),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_BUGLE_MELLOPHONE_BUGLE, mx::api::SoundID::brassBugleMellophoneBugle),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_BUGLE_SOPRANO, mx::api::SoundID::brassBugleSoprano),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_CIMBASSO, mx::api::SoundID::brassCimbasso),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_CONCH_SHELL, mx::api::SoundID::brassConchShell),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_CORNET, mx::api::SoundID::brassCornet),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_CORNET_SOPRANO, mx::api::SoundID::brassCornetSoprano),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_CORNETT, mx::api::SoundID::brassCornett),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_CORNETT_TENOR, mx::api::SoundID::brassCornettTenor),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_CORNETTINO, mx::api::SoundID::brassCornettino),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_DIDGERIDOO, mx::api::SoundID::brassDidgeridoo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_EUPHONIUM, mx::api::SoundID::brassEuphonium),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_FISCORN, mx::api::SoundID::brassFiscorn),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_FLUGELHORN, mx::api::SoundID::brassFlugelhorn),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_FRENCH_HORN, mx::api::SoundID::brassFrenchHorn),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_GROUP, mx::api::SoundID::brassGroup),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_GROUP_SYNTH, mx::api::SoundID::brassGroupSynth),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_HELICON, mx::api::SoundID::brassHelicon),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_HORAGAI, mx::api::SoundID::brassHoragai),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_KUHLOHORN, mx::api::SoundID::brassKuhlohorn),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_MELLOPHONE, mx::api::SoundID::brassMellophone),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_NATURAL_HORN, mx::api::SoundID::brassNaturalHorn),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_OPHICLEIDE, mx::api::SoundID::brassOphicleide),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_POSTHORN, mx::api::SoundID::brassPosthorn),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_RAG_DUNG, mx::api::SoundID::brassRagDung),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_SACKBUTT, mx::api::SoundID::brassSackbutt),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_SACKBUTT_ALTO, mx::api::SoundID::brassSackbuttAlto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_SACKBUTT_BASS, mx::api::SoundID::brassSackbuttBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_SACKBUTT_TENOR, mx::api::SoundID::brassSackbuttTenor),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_SAXHORN, mx::api::SoundID::brassSaxhorn),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_SERPENT, mx::api::SoundID::brassSerpent),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_SHOFAR, mx::api::SoundID::brassShofar),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_SOUSAPHONE, mx::api::SoundID::brassSousaphone),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_TROMBONE, mx::api::SoundID::brassTrombone),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_TROMBONE_ALTO, mx::api::SoundID::brassTromboneAlto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_TROMBONE_BASS, mx::api::SoundID::brassTromboneBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_TROMBONE_CONTRABASS, mx::api::SoundID::brassTromboneContrabass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_TROMBONE_TENOR, mx::api::SoundID::brassTromboneTenor),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_TRUMPET, mx::api::SoundID::brassTrumpet),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_TRUMPET_BAROQUE, mx::api::SoundID::brassTrumpetBaroque),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_TRUMPET_BASS, mx::api::SoundID::brassTrumpetBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_TRUMPET_BFLAT, mx::api::SoundID::brassTrumpetBflat),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_TRUMPET_C, mx::api::SoundID::brassTrumpetC),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_TRUMPET_D, mx::api::SoundID::brassTrumpetD),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_TRUMPET_PICCOLO, mx::api::SoundID::brassTrumpetPiccolo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_TRUMPET_POCKET, mx::api::SoundID::brassTrumpetPocket),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_TRUMPET_SLIDE, mx::api::SoundID::brassTrumpetSlide),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_TRUMPET_TENOR, mx::api::SoundID::brassTrumpetTenor),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_TUBA, mx::api::SoundID::brassTuba),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_TUBA_BASS, mx::api::SoundID::brassTubaBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_TUBA_SUBCONTRABASS, mx::api::SoundID::brassTubaSubcontrabass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_VIENNA_HORN, mx::api::SoundID::brassViennaHorn),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_VUVUZELA, mx::api::SoundID::brassVuvuzela),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::BRASS_WAGNER_TUBA, mx::api::SoundID::brassWagnerTuba),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_APENTEMMA, mx::api::SoundID::drumApentemma),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_ASHIKO, mx::api::SoundID::drumAshiko),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_ATABAQUE, mx::api::SoundID::drumAtabaque),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_ATOKE, mx::api::SoundID::drumAtoke),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_ATSIMEVU, mx::api::SoundID::drumAtsimevu),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_AXATSE, mx::api::SoundID::drumAxatse),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_BASS_DRUM, mx::api::SoundID::drumBassDrum),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_BATA, mx::api::SoundID::drumBata),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_BATA_ITOTELE, mx::api::SoundID::drumBataItotele),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_BATA_IYA, mx::api::SoundID::drumBataIya),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_BATA_OKONKOLO, mx::api::SoundID::drumBataOkonkolo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_BENDIR, mx::api::SoundID::drumBendir),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_BODHRAN, mx::api::SoundID::drumBodhran),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_BOMBO, mx::api::SoundID::drumBombo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_BONGO, mx::api::SoundID::drumBongo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_BOUGARABOU, mx::api::SoundID::drumBougarabou),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_BUFFALO_DRUM, mx::api::SoundID::drumBuffaloDrum),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_CAJON, mx::api::SoundID::drumCajon),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_CHENDA, mx::api::SoundID::drumChenda),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_CHU_DAIKO, mx::api::SoundID::drumChuDaiko),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_CONGA, mx::api::SoundID::drumConga),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_CUICA, mx::api::SoundID::drumCuica),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_DABAKAN, mx::api::SoundID::drumDabakan),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_DAFF, mx::api::SoundID::drumDaff),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_DAFLI, mx::api::SoundID::drumDafli),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_DAIBYOSI, mx::api::SoundID::drumDaibyosi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_DAMROO, mx::api::SoundID::drumDamroo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_DARABUKA, mx::api::SoundID::drumDarabuka),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_DEF, mx::api::SoundID::drumDef),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_DHOL, mx::api::SoundID::drumDhol),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_DHOLAK, mx::api::SoundID::drumDholak),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_DJEMBE, mx::api::SoundID::drumDjembe),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_DOIRA, mx::api::SoundID::drumDoira),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_DONDO, mx::api::SoundID::drumDondo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_DOUN_DOUN_BA, mx::api::SoundID::drumDounDounBa),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_DUFF, mx::api::SoundID::drumDuff),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_DUMBEK, mx::api::SoundID::drumDumbek),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_FONTOMFROM, mx::api::SoundID::drumFontomfrom),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_FRAME_DRUM, mx::api::SoundID::drumFrameDrum),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_FRAME_DRUM_ARABIAN, mx::api::SoundID::drumFrameDrumArabian),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_GEDUK, mx::api::SoundID::drumGeduk),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_GHATAM, mx::api::SoundID::drumGhatam),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_GOME, mx::api::SoundID::drumGome),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_GROUP, mx::api::SoundID::drumGroup),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_GROUP_CHINESE, mx::api::SoundID::drumGroupChinese),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_GROUP_EWE, mx::api::SoundID::drumGroupEwe),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_GROUP_INDIAN, mx::api::SoundID::drumGroupIndian),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_GROUP_SET, mx::api::SoundID::drumGroupSet),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_HAND_DRUM, mx::api::SoundID::drumHandDrum),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_HIRA_DAIKO, mx::api::SoundID::drumHiraDaiko),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_IBO, mx::api::SoundID::drumIbo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_IGIHUMURIZO, mx::api::SoundID::drumIgihumurizo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_INYAHURA, mx::api::SoundID::drumInyahura),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_ISHAKWE, mx::api::SoundID::drumIshakwe),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_JANG_GU, mx::api::SoundID::drumJangGu),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_KAGAN, mx::api::SoundID::drumKagan),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_KAKKO, mx::api::SoundID::drumKakko),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_KANJIRA, mx::api::SoundID::drumKanjira),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_KENDHANG, mx::api::SoundID::drumKendhang),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_KENDHANG_AGENG, mx::api::SoundID::drumKendhangAgeng),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_KENDHANG_CIBLON, mx::api::SoundID::drumKendhangCiblon),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_KENKENI, mx::api::SoundID::drumKenkeni),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_KHOL, mx::api::SoundID::drumKhol),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_KICK_DRUM, mx::api::SoundID::drumKickDrum),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_KIDI, mx::api::SoundID::drumKidi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_KO_DAIKO, mx::api::SoundID::drumKoDaiko),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_KPANLOGO, mx::api::SoundID::drumKpanlogo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_KUDUM, mx::api::SoundID::drumKudum),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_LAMBEG, mx::api::SoundID::drumLambeg),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_LION_DRUM, mx::api::SoundID::drumLionDrum),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_LOG_DRUM, mx::api::SoundID::drumLogDrum),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_LOG_DRUM_AFRICAN, mx::api::SoundID::drumLogDrumAfrican),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_LOG_DRUM_NATIVE, mx::api::SoundID::drumLogDrumNative),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_LOG_DRUM_NIGERIAN, mx::api::SoundID::drumLogDrumNigerian),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_MADAL, mx::api::SoundID::drumMadal),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_MADDALE, mx::api::SoundID::drumMaddale),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_MRIDANGAM, mx::api::SoundID::drumMridangam),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_NAAL, mx::api::SoundID::drumNaal),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_NAGADO_DAIKO, mx::api::SoundID::drumNagadoDaiko),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_NAGARA, mx::api::SoundID::drumNagara),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_NAQARA, mx::api::SoundID::drumNaqara),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_O_DAIKO, mx::api::SoundID::drumODaiko),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_OKAWA, mx::api::SoundID::drumOkawa),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_OKEDO_DAIKO, mx::api::SoundID::drumOkedoDaiko),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_PAHU_HULA, mx::api::SoundID::drumPahuHula),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_PAKHAWAJ, mx::api::SoundID::drumPakhawaj),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_PANDEIRO, mx::api::SoundID::drumPandeiro),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_PANDERO, mx::api::SoundID::drumPandero),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_POWWOW, mx::api::SoundID::drumPowwow),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_PUEBLO, mx::api::SoundID::drumPueblo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_REPINIQUE, mx::api::SoundID::drumRepinique),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_RIQ, mx::api::SoundID::drumRiq),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_ROTOTOM, mx::api::SoundID::drumRototom),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_SABAR, mx::api::SoundID::drumSabar),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_SAKARA, mx::api::SoundID::drumSakara),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_SAMPHO, mx::api::SoundID::drumSampho),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_SANGBAN, mx::api::SoundID::drumSangban),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_SHIME_DAIKO, mx::api::SoundID::drumShimeDaiko),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_SLIT_DRUM, mx::api::SoundID::drumSlitDrum),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_SLIT_DRUM_KRIN, mx::api::SoundID::drumSlitDrumKrin),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_SNARE_DRUM, mx::api::SoundID::drumSnareDrum),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_SNARE_DRUM_ELECTRIC, mx::api::SoundID::drumSnareDrumElectric),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_SOGO, mx::api::SoundID::drumSogo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_SURDO, mx::api::SoundID::drumSurdo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TABLA, mx::api::SoundID::drumTabla),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TABLA_BAYAN, mx::api::SoundID::drumTablaBayan),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TABLA_DAYAN, mx::api::SoundID::drumTablaDayan),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TAIKO, mx::api::SoundID::drumTaiko),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TALKING, mx::api::SoundID::drumTalking),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TAMA, mx::api::SoundID::drumTama),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TAMBORITA, mx::api::SoundID::drumTamborita),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TAMBOURINE, mx::api::SoundID::drumTambourine),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TAMTE, mx::api::SoundID::drumTamte),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TANGKU, mx::api::SoundID::drumTangku),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TAN_TAN, mx::api::SoundID::drumTanTan),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TAPHON, mx::api::SoundID::drumTaphon),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TAR, mx::api::SoundID::drumTar),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TASHA, mx::api::SoundID::drumTasha),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TENOR_DRUM, mx::api::SoundID::drumTenorDrum),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TEPONAXTLI, mx::api::SoundID::drumTeponaxtli),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_THAVIL, mx::api::SoundID::drumThavil),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_THE_BOX, mx::api::SoundID::drumTheBox),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TIMBALE, mx::api::SoundID::drumTimbale),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TIMPANI, mx::api::SoundID::drumTimpani),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TINAJA, mx::api::SoundID::drumTinaja),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TOERE, mx::api::SoundID::drumToere),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TOMBAK, mx::api::SoundID::drumTombak),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TOM_TOM, mx::api::SoundID::drumTomTom),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TOM_TOM_SYNTH, mx::api::SoundID::drumTomTomSynth),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TSUZUMI, mx::api::SoundID::drumTsuzumi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_TUMBAK, mx::api::SoundID::drumTumbak),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_UCHIWA_DAIKO, mx::api::SoundID::drumUchiwaDaiko),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_UDAKU, mx::api::SoundID::drumUdaku),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_UDU, mx::api::SoundID::drumUdu),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::DRUM_ZARB, mx::api::SoundID::drumZarb),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_AEOLIAN_HARP, mx::api::SoundID::effectAeolianHarp),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_AIR_HORN, mx::api::SoundID::effectAirHorn),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_APPLAUSE, mx::api::SoundID::effectApplause),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_BASS_STRING_SLAP, mx::api::SoundID::effectBassStringSlap),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_BIRD, mx::api::SoundID::effectBird),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_BIRD_NIGHTINGALE, mx::api::SoundID::effectBirdNightingale),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_BIRD_TWEET, mx::api::SoundID::effectBirdTweet),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_BREATH, mx::api::SoundID::effectBreath),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_BUBBLE, mx::api::SoundID::effectBubble),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_BULLROARER, mx::api::SoundID::effectBullroarer),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_BURST, mx::api::SoundID::effectBurst),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_CAR, mx::api::SoundID::effectCar),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_CAR_CRASH, mx::api::SoundID::effectCarCrash),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_CAR_ENGINE, mx::api::SoundID::effectCarEngine),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_CAR_PASS, mx::api::SoundID::effectCarPass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_CAR_STOP, mx::api::SoundID::effectCarStop),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_CRICKETS, mx::api::SoundID::effectCrickets),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_DOG, mx::api::SoundID::effectDog),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_DOOR_CREAK, mx::api::SoundID::effectDoorCreak),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_DOOR_SLAM, mx::api::SoundID::effectDoorSlam),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_EXPLOSION, mx::api::SoundID::effectExplosion),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_FLUTE_KEY_CLICK, mx::api::SoundID::effectFluteKeyClick),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_FOOTSTEPS, mx::api::SoundID::effectFootsteps),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_FROGS, mx::api::SoundID::effectFrogs),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_GUITAR_CUTTING, mx::api::SoundID::effectGuitarCutting),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_GUITAR_FRET, mx::api::SoundID::effectGuitarFret),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_GUNSHOT, mx::api::SoundID::effectGunshot),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_HAND_CLAP, mx::api::SoundID::effectHandClap),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_HEARTBEAT, mx::api::SoundID::effectHeartbeat),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_HELICOPTER, mx::api::SoundID::effectHelicopter),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_HIGH_Q, mx::api::SoundID::effectHighQ),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_HORSE_GALLOP, mx::api::SoundID::effectHorseGallop),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_JET_PLANE, mx::api::SoundID::effectJetPlane),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_LASER_GUN, mx::api::SoundID::effectLaserGun),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_LAUGH, mx::api::SoundID::effectLaugh),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_LIONS_ROAR, mx::api::SoundID::effectLionsRoar),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_MACHINE_GUN, mx::api::SoundID::effectMachineGun),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_MARCHING_MACHINE, mx::api::SoundID::effectMarchingMachine),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_METRONOME_BELL, mx::api::SoundID::effectMetronomeBell),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_METRONOME_CLICK, mx::api::SoundID::effectMetronomeClick),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_PAT, mx::api::SoundID::effectPat),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_PUNCH, mx::api::SoundID::effectPunch),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_RAIN, mx::api::SoundID::effectRain),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_SCRATCH, mx::api::SoundID::effectScratch),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_SCREAM, mx::api::SoundID::effectScream),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_SEASHORE, mx::api::SoundID::effectSeashore),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_SIREN, mx::api::SoundID::effectSiren),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_SLAP, mx::api::SoundID::effectSlap),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_SNAP, mx::api::SoundID::effectSnap),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_STAMP, mx::api::SoundID::effectStamp),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_STARSHIP, mx::api::SoundID::effectStarship),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_STREAM, mx::api::SoundID::effectStream),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_TELEPHONE_RING, mx::api::SoundID::effectTelephoneRing),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_THUNDER, mx::api::SoundID::effectThunder),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_TRAIN, mx::api::SoundID::effectTrain),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_TRASH_CAN, mx::api::SoundID::effectTrashCan),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_WHIP, mx::api::SoundID::effectWhip),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_WHISTLE, mx::api::SoundID::effectWhistle),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_WHISTLE_MOUTH_SIREN, mx::api::SoundID::effectWhistleMouthSiren),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_WHISTLE_POLICE, mx::api::SoundID::effectWhistlePolice),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_WHISTLE_SLIDE, mx::api::SoundID::effectWhistleSlide),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_WHISTLE_TRAIN, mx::api::SoundID::effectWhistleTrain),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::EFFECT_WIND, mx::api::SoundID::effectWind),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_ACCORDION, mx::api::SoundID::keyboardAccordion),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_BANDONEON, mx::api::SoundID::keyboardBandoneon),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_CELESTA, mx::api::SoundID::keyboardCelesta),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_CLAVICHORD, mx::api::SoundID::keyboardClavichord),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_CLAVICHORD_SYNTH, mx::api::SoundID::keyboardClavichordSynth),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_CONCERTINA, mx::api::SoundID::keyboardConcertina),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_FORTEPIANO, mx::api::SoundID::keyboardFortepiano),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_HARMONIUM, mx::api::SoundID::keyboardHarmonium),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_HARPSICHORD, mx::api::SoundID::keyboardHarpsichord),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_ONDES_MARTENOT, mx::api::SoundID::keyboardOndesMartenot),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_ORGAN, mx::api::SoundID::keyboardOrgan),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_ORGAN_DRAWBAR, mx::api::SoundID::keyboardOrganDrawbar),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_ORGAN_PERCUSSIVE, mx::api::SoundID::keyboardOrganPercussive),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_ORGAN_PIPE, mx::api::SoundID::keyboardOrganPipe),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_ORGAN_REED, mx::api::SoundID::keyboardOrganReed),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_ORGAN_ROTARY, mx::api::SoundID::keyboardOrganRotary),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_PIANO, mx::api::SoundID::keyboardPiano),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_PIANO_ELECTRIC, mx::api::SoundID::keyboardPianoElectric),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_PIANO_GRAND, mx::api::SoundID::keyboardPianoGrand),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_PIANO_HONKY_TONK, mx::api::SoundID::keyboardPianoHonkyTonk),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_PIANO_PREPARED, mx::api::SoundID::keyboardPianoPrepared),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_PIANO_TOY, mx::api::SoundID::keyboardPianoToy),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_PIANO_UPRIGHT, mx::api::SoundID::keyboardPianoUpright),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::KEYBOARD_VIRGINAL, mx::api::SoundID::keyboardVirginal),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_ADODO, mx::api::SoundID::metalAdodo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_ANVIL, mx::api::SoundID::metalAnvil),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BABENDIL, mx::api::SoundID::metalBabendil),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_AGOGO, mx::api::SoundID::metalBellsAgogo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_ALMGLOCKEN, mx::api::SoundID::metalBellsAlmglocken),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_BELL_PLATE, mx::api::SoundID::metalBellsBellPlate),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_BELL_TREE, mx::api::SoundID::metalBellsBellTree),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_CARILLON, mx::api::SoundID::metalBellsCarillon),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_CHIMES, mx::api::SoundID::metalBellsChimes),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_CHIMTA, mx::api::SoundID::metalBellsChimta),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_CHIPPLI, mx::api::SoundID::metalBellsChippli),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_CHURCH, mx::api::SoundID::metalBellsChurch),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_COWBELL, mx::api::SoundID::metalBellsCowbell),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_DAWURO, mx::api::SoundID::metalBellsDawuro),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_GANKOKWE, mx::api::SoundID::metalBellsGankokwe),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_GHUNGROO, mx::api::SoundID::metalBellsGhungroo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_HATHELI, mx::api::SoundID::metalBellsHatheli),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_JINGLE_BELL, mx::api::SoundID::metalBellsJingleBell),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_KHARTAL, mx::api::SoundID::metalBellsKhartal),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_MARK_TREE, mx::api::SoundID::metalBellsMarkTree),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_SISTRUM, mx::api::SoundID::metalBellsSistrum),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_SLEIGH_BELLS, mx::api::SoundID::metalBellsSleighBells),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_TEMPLE, mx::api::SoundID::metalBellsTemple),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_TIBETAN, mx::api::SoundID::metalBellsTibetan),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_TINKLEBELL, mx::api::SoundID::metalBellsTinklebell),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_TRYCHEL, mx::api::SoundID::metalBellsTrychel),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_WIND_CHIMES, mx::api::SoundID::metalBellsWindChimes),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BELLS_ZILLS, mx::api::SoundID::metalBellsZills),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BERIMBAU, mx::api::SoundID::metalBerimbau),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_BRAKE_DRUMS, mx::api::SoundID::metalBrakeDrums),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CROTALES, mx::api::SoundID::metalCrotales),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CYMBAL_BO, mx::api::SoundID::metalCymbalBo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CYMBAL_CENG_CENG, mx::api::SoundID::metalCymbalCengCeng),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CYMBAL_CHABARA, mx::api::SoundID::metalCymbalChabara),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CYMBAL_CHINESE, mx::api::SoundID::metalCymbalChinese),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CYMBAL_CHING, mx::api::SoundID::metalCymbalChing),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CYMBAL_CLASH, mx::api::SoundID::metalCymbalClash),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CYMBAL_CRASH, mx::api::SoundID::metalCymbalCrash),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CYMBAL_FINGER, mx::api::SoundID::metalCymbalFinger),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CYMBAL_HAND, mx::api::SoundID::metalCymbalHand),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CYMBAL_KESI, mx::api::SoundID::metalCymbalKesi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CYMBAL_MANJEERA, mx::api::SoundID::metalCymbalManjeera),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CYMBAL_REVERSE, mx::api::SoundID::metalCymbalReverse),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CYMBAL_RIDE, mx::api::SoundID::metalCymbalRide),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CYMBAL_SIZZLE, mx::api::SoundID::metalCymbalSizzle),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CYMBAL_SPLASH, mx::api::SoundID::metalCymbalSplash),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CYMBAL_SUSPENDED, mx::api::SoundID::metalCymbalSuspended),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CYMBAL_TEBYOSHI, mx::api::SoundID::metalCymbalTebyoshi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CYMBAL_TIBETAN, mx::api::SoundID::metalCymbalTibetan),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_CYMBAL_TINGSHA, mx::api::SoundID::metalCymbalTingsha),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_FLEXATONE, mx::api::SoundID::metalFlexatone),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_GONG, mx::api::SoundID::metalGong),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_GONG_AGENG, mx::api::SoundID::metalGongAgeng),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_GONG_AGUNG, mx::api::SoundID::metalGongAgung),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_GONG_CHANCHIKI, mx::api::SoundID::metalGongChanchiki),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_GONG_CHINESE, mx::api::SoundID::metalGongChinese),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_GONG_GANDINGAN, mx::api::SoundID::metalGongGandingan),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_GONG_KEMPUL, mx::api::SoundID::metalGongKempul),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_GONG_KEMPYANG, mx::api::SoundID::metalGongKempyang),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_GONG_KETUK, mx::api::SoundID::metalGongKetuk),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_GONG_KKWENGGWARI, mx::api::SoundID::metalGongKkwenggwari),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_GONG_LUO, mx::api::SoundID::metalGongLuo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_GONG_SINGING, mx::api::SoundID::metalGongSinging),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_GONG_THAI, mx::api::SoundID::metalGongThai),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_GUIRA, mx::api::SoundID::metalGuira),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_HANG, mx::api::SoundID::metalHang),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_HI_HAT, mx::api::SoundID::metalHiHat),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_JAW_HARP, mx::api::SoundID::metalJawHarp),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_KENGONG, mx::api::SoundID::metalKengong),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_MURCHANG, mx::api::SoundID::metalMurchang),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_MUSICAL_SAW, mx::api::SoundID::metalMusicalSaw),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_SINGING_BOWL, mx::api::SoundID::metalSingingBowl),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_SPOONS, mx::api::SoundID::metalSpoons),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_STEEL_DRUMS, mx::api::SoundID::metalSteelDrums),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_TAMTAM, mx::api::SoundID::metalTamtam),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_THUNDERSHEET, mx::api::SoundID::metalThundersheet),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_TRIANGLE, mx::api::SoundID::metalTriangle),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::METAL_WASHBOARD, mx::api::SoundID::metalWashboard),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_ANGKLUNG, mx::api::SoundID::pitchedPercussionAngklung),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_BALAFON, mx::api::SoundID::pitchedPercussionBalafon),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_BELL_LYRE, mx::api::SoundID::pitchedPercussionBellLyre),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_BELLS, mx::api::SoundID::pitchedPercussionBells),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_BIANQING, mx::api::SoundID::pitchedPercussionBianqing),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_BIANZHONG, mx::api::SoundID::pitchedPercussionBianzhong),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_BONANG, mx::api::SoundID::pitchedPercussionBonang),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_CIMBALOM, mx::api::SoundID::pitchedPercussionCimbalom),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_CRYSTAL_GLASSES, mx::api::SoundID::pitchedPercussionCrystalGlasses),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_DAN_TAM_THAP_LUC, mx::api::SoundID::pitchedPercussionDanTamThapLuc),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_FANGXIANG, mx::api::SoundID::pitchedPercussionFangxiang),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_GANDINGAN_A_KAYO, mx::api::SoundID::pitchedPercussionGandinganAKayo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_GANGSA, mx::api::SoundID::pitchedPercussionGangsa),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_GENDER, mx::api::SoundID::pitchedPercussionGender),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_GIYING, mx::api::SoundID::pitchedPercussionGiying),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_GLASS_HARMONICA, mx::api::SoundID::pitchedPercussionGlassHarmonica),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_GLOCKENSPIEL, mx::api::SoundID::pitchedPercussionGlockenspiel),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_GLOCKENSPIEL_ALTO, mx::api::SoundID::pitchedPercussionGlockenspielAlto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_GLOCKENSPIEL_SOPRANO, mx::api::SoundID::pitchedPercussionGlockenspielSoprano),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_GYIL, mx::api::SoundID::pitchedPercussionGyil),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_HAMMER_DULCIMER, mx::api::SoundID::pitchedPercussionHammerDulcimer),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_HANDBELLS, mx::api::SoundID::pitchedPercussionHandbells),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_KALIMBA, mx::api::SoundID::pitchedPercussionKalimba),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_KANTIL, mx::api::SoundID::pitchedPercussionKantil),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_KHIM, mx::api::SoundID::pitchedPercussionKhim),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_KULINTANG, mx::api::SoundID::pitchedPercussionKulintang),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_KULINTANG_A_KAYO, mx::api::SoundID::pitchedPercussionKulintangAKayo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_KULINTANG_A_TINIOK, mx::api::SoundID::pitchedPercussionKulintangATiniok),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_LIKEMBE, mx::api::SoundID::pitchedPercussionLikembe),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_LUNTANG, mx::api::SoundID::pitchedPercussionLuntang),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_MARIMBA, mx::api::SoundID::pitchedPercussionMarimba),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_MARIMBA_BASS, mx::api::SoundID::pitchedPercussionMarimbaBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_MBIRA, mx::api::SoundID::pitchedPercussionMbira),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_MBIRA_ARRAY, mx::api::SoundID::pitchedPercussionMbiraArray),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_METALLOPHONE, mx::api::SoundID::pitchedPercussionMetallophone),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_METALLOPHONE_ALTO, mx::api::SoundID::pitchedPercussionMetallophoneAlto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_METALLOPHONE_BASS, mx::api::SoundID::pitchedPercussionMetallophoneBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_METALLOPHONE_SOPRANO, mx::api::SoundID::pitchedPercussionMetallophoneSoprano),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_MUSIC_BOX, mx::api::SoundID::pitchedPercussionMusicBox),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_PELOG_PANERUS, mx::api::SoundID::pitchedPercussionPelogPanerus),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_PEMADE, mx::api::SoundID::pitchedPercussionPemade),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_PENYACAH, mx::api::SoundID::pitchedPercussionPenyacah),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_RANAT_EK, mx::api::SoundID::pitchedPercussionRanatEk),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_RANAT_EK_LEK, mx::api::SoundID::pitchedPercussionRanatEkLek),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_RANAT_THUM, mx::api::SoundID::pitchedPercussionRanatThum),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_RANAT_THUM_LEK, mx::api::SoundID::pitchedPercussionRanatThumLek),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_REYONG, mx::api::SoundID::pitchedPercussionReyong),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_SANZA, mx::api::SoundID::pitchedPercussionSanza),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_SARON_BARUNG, mx::api::SoundID::pitchedPercussionSaronBarung),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_SARON_DEMONG, mx::api::SoundID::pitchedPercussionSaronDemong),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_SARON_PANERUS, mx::api::SoundID::pitchedPercussionSaronPanerus),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_SLENDRO_PANERUS, mx::api::SoundID::pitchedPercussionSlendroPanerus),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_SLENTEM, mx::api::SoundID::pitchedPercussionSlentem),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_TSYMBALY, mx::api::SoundID::pitchedPercussionTsymbaly),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_TUBES, mx::api::SoundID::pitchedPercussionTubes),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_TUBULAR_BELLS, mx::api::SoundID::pitchedPercussionTubularBells),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_VIBRAPHONE, mx::api::SoundID::pitchedPercussionVibraphone),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_XYLOPHONE, mx::api::SoundID::pitchedPercussionXylophone),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_XYLOPHONE_ALTO, mx::api::SoundID::pitchedPercussionXylophoneAlto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_XYLOPHONE_BASS, mx::api::SoundID::pitchedPercussionXylophoneBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_XYLOPHONE_SOPRANO, mx::api::SoundID::pitchedPercussionXylophoneSoprano),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_XYLORIMBA, mx::api::SoundID::pitchedPercussionXylorimba),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PITCHED_PERCUSSION_YANGQIN, mx::api::SoundID::pitchedPercussionYangqin),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_ARCHLUTE, mx::api::SoundID::pluckArchlute),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_AUTOHARP, mx::api::SoundID::pluckAutoharp),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BAGLAMA, mx::api::SoundID::pluckBaglama),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BAJO, mx::api::SoundID::pluckBajo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BALALAIKA, mx::api::SoundID::pluckBalalaika),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BALALAIKA_ALTO, mx::api::SoundID::pluckBalalaikaAlto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BALALAIKA_BASS, mx::api::SoundID::pluckBalalaikaBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BALALAIKA_CONTRABASS, mx::api::SoundID::pluckBalalaikaContrabass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BALALAIKA_PICCOLO, mx::api::SoundID::pluckBalalaikaPiccolo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BALALAIKA_PRIMA, mx::api::SoundID::pluckBalalaikaPrima),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BALALAIKA_SECUNDA, mx::api::SoundID::pluckBalalaikaSecunda),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BANDOLA, mx::api::SoundID::pluckBandola),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BANDURA, mx::api::SoundID::pluckBandura),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BANDURRIA, mx::api::SoundID::pluckBandurria),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BANJO, mx::api::SoundID::pluckBanjo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BANJO_TENOR, mx::api::SoundID::pluckBanjoTenor),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BANJOLELE, mx::api::SoundID::pluckBanjolele),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BARBAT, mx::api::SoundID::pluckBarbat),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BASS, mx::api::SoundID::pluckBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BASS_ACOUSTIC, mx::api::SoundID::pluckBassAcoustic),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BASS_BOLON, mx::api::SoundID::pluckBassBolon),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BASS_ELECTRIC, mx::api::SoundID::pluckBassElectric),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BASS_FRETLESS, mx::api::SoundID::pluckBassFretless),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BASS_GUITARRON, mx::api::SoundID::pluckBassGuitarron),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BASS_SYNTH, mx::api::SoundID::pluckBassSynth),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BASS_SYNTH_LEAD, mx::api::SoundID::pluckBassSynthLead),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BASS_WASHTUB, mx::api::SoundID::pluckBassWashtub),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BASS_WHAMOLA, mx::api::SoundID::pluckBassWhamola),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BEGENA, mx::api::SoundID::pluckBegena),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BIWA, mx::api::SoundID::pluckBiwa),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BORDONUA, mx::api::SoundID::pluckBordonua),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BOUZOUKI, mx::api::SoundID::pluckBouzouki),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_BOUZOUKI_IRISH, mx::api::SoundID::pluckBouzoukiIrish),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_CELTIC_HARP, mx::api::SoundID::pluckCelticHarp),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_CHARANGO, mx::api::SoundID::pluckCharango),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_CHITARRA_BATTENTE, mx::api::SoundID::pluckChitarraBattente),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_CITHARA, mx::api::SoundID::pluckCithara),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_CITTERN, mx::api::SoundID::pluckCittern),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_CUATRO, mx::api::SoundID::pluckCuatro),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_DAN_BAU, mx::api::SoundID::pluckDanBau),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_DAN_NGUYET, mx::api::SoundID::pluckDanNguyet),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_DAN_TRANH, mx::api::SoundID::pluckDanTranh),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_DAN_TY_BA, mx::api::SoundID::pluckDanTyBa),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_DIDDLEY_BOW, mx::api::SoundID::pluckDiddleyBow),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_DOMRA, mx::api::SoundID::pluckDomra),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_DOMU, mx::api::SoundID::pluckDomu),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_DULCIMER, mx::api::SoundID::pluckDulcimer),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_DUTAR, mx::api::SoundID::pluckDutar),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_DUXIANQIN, mx::api::SoundID::pluckDuxianqin),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_EKTARA, mx::api::SoundID::pluckEktara),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_GEOMUNGO, mx::api::SoundID::pluckGeomungo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_GOTTUVADHYAM, mx::api::SoundID::pluckGottuvadhyam),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_GUITAR, mx::api::SoundID::pluckGuitar),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_GUITAR_ACOUSTIC, mx::api::SoundID::pluckGuitarAcoustic),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_GUITAR_ELECTRIC, mx::api::SoundID::pluckGuitarElectric),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_GUITAR_NYLON_STRING, mx::api::SoundID::pluckGuitarNylonString),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_GUITAR_PEDAL_STEEL, mx::api::SoundID::pluckGuitarPedalSteel),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_GUITAR_PORTUGUESE, mx::api::SoundID::pluckGuitarPortuguese),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_GUITAR_REQUINTO, mx::api::SoundID::pluckGuitarRequinto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_GUITAR_RESONATOR, mx::api::SoundID::pluckGuitarResonator),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_GUITAR_STEEL_STRING, mx::api::SoundID::pluckGuitarSteelString),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_GUITJO, mx::api::SoundID::pluckGuitjo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_GUITJO_DOUBLE_NECK, mx::api::SoundID::pluckGuitjoDoubleNeck),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_GUQIN, mx::api::SoundID::pluckGuqin),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_GUZHENG, mx::api::SoundID::pluckGuzheng),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_GUZHENG_CHOAZHOU, mx::api::SoundID::pluckGuzhengChoazhou),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_HARP, mx::api::SoundID::pluckHarp),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_HARP_GUITAR, mx::api::SoundID::pluckHarpGuitar),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_HUAPANGUERA, mx::api::SoundID::pluckHuapanguera),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_JARANA_HUASTECA, mx::api::SoundID::pluckJaranaHuasteca),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_JARANA_JAROCHA, mx::api::SoundID::pluckJaranaJarocha),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_JARANA_JAROCHA_MOSQUITO, mx::api::SoundID::pluckJaranaJarochaMosquito),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_JARANA_JAROCHA_PRIMERA, mx::api::SoundID::pluckJaranaJarochaPrimera),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_JARANA_JAROCHA_SEGUNDA, mx::api::SoundID::pluckJaranaJarochaSegunda),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_JARANA_JAROCHA_TERCERA, mx::api::SoundID::pluckJaranaJarochaTercera),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_KABOSY, mx::api::SoundID::pluckKabosy),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_KANTELE, mx::api::SoundID::pluckKantele),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_KANUN, mx::api::SoundID::pluckKanun),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_KAYAGUM, mx::api::SoundID::pluckKayagum),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_KOBZA, mx::api::SoundID::pluckKobza),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_KOMUZ, mx::api::SoundID::pluckKomuz),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_KORA, mx::api::SoundID::pluckKora),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_KOTO, mx::api::SoundID::pluckKoto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_KUTIYAPI, mx::api::SoundID::pluckKutiyapi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_LANGELEIK, mx::api::SoundID::pluckLangeleik),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_LAUD, mx::api::SoundID::pluckLaud),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_LUTE, mx::api::SoundID::pluckLute),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_LYRE, mx::api::SoundID::pluckLyre),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_MANDOBASS, mx::api::SoundID::pluckMandobass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_MANDOCELLO, mx::api::SoundID::pluckMandocello),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_MANDOLA, mx::api::SoundID::pluckMandola),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_MANDOLIN, mx::api::SoundID::pluckMandolin),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_MANDOLIN_OCTAVE, mx::api::SoundID::pluckMandolinOctave),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_MANDORA, mx::api::SoundID::pluckMandora),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_MANDORE, mx::api::SoundID::pluckMandore),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_MAROVANY, mx::api::SoundID::pluckMarovany),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_MUSICAL_BOW, mx::api::SoundID::pluckMusicalBow),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_NGONI, mx::api::SoundID::pluckNgoni),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_OUD, mx::api::SoundID::pluckOud),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_PIPA, mx::api::SoundID::pluckPipa),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_PSALTERY, mx::api::SoundID::pluckPsaltery),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_RUAN, mx::api::SoundID::pluckRuan),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_SALLANEH, mx::api::SoundID::pluckSallaneh),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_SANSHIN, mx::api::SoundID::pluckSanshin),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_SANTOOR, mx::api::SoundID::pluckSantoor),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_SANXIAN, mx::api::SoundID::pluckSanxian),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_SAROD, mx::api::SoundID::pluckSarod),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_SAUNG, mx::api::SoundID::pluckSaung),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_SAZ, mx::api::SoundID::pluckSaz),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_SE, mx::api::SoundID::pluckSe),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_SETAR, mx::api::SoundID::pluckSetar),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_SHAMISEN, mx::api::SoundID::pluckShamisen),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_SITAR, mx::api::SoundID::pluckSitar),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_SYNTH, mx::api::SoundID::pluckSynth),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_SYNTH_CHARANG, mx::api::SoundID::pluckSynthCharang),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_SYNTH_CHIFF, mx::api::SoundID::pluckSynthChiff),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_SYNTH_STICK, mx::api::SoundID::pluckSynthStick),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_TAMBURA, mx::api::SoundID::pluckTambura),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_TAMBURA_BULGARIAN, mx::api::SoundID::pluckTamburaBulgarian),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_TAMBURA_FEMALE, mx::api::SoundID::pluckTamburaFemale),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_TAMBURA_MALE, mx::api::SoundID::pluckTamburaMale),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_TAR, mx::api::SoundID::pluckTar),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_THEORBO, mx::api::SoundID::pluckTheorbo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_TIMPLE, mx::api::SoundID::pluckTimple),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_TIPLE, mx::api::SoundID::pluckTiple),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_TRES, mx::api::SoundID::pluckTres),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_UKULELE, mx::api::SoundID::pluckUkulele),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_UKULELE_TENOR, mx::api::SoundID::pluckUkuleleTenor),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_VALIHA, mx::api::SoundID::pluckValiha),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_VEENA, mx::api::SoundID::pluckVeena),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_VEENA_MOHAN, mx::api::SoundID::pluckVeenaMohan),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_VEENA_RUDRA, mx::api::SoundID::pluckVeenaRudra),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_VEENA_VICHITRA, mx::api::SoundID::pluckVeenaVichitra),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_VIHUELA, mx::api::SoundID::pluckVihuela),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_VIHUELA_MEXICAN, mx::api::SoundID::pluckVihuelaMexican),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_XALAM, mx::api::SoundID::pluckXalam),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_YUEQIN, mx::api::SoundID::pluckYueqin),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_ZITHER, mx::api::SoundID::pluckZither),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::PLUCK_ZITHER_OVERTONE, mx::api::SoundID::pluckZitherOvertone),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_AFOXE, mx::api::SoundID::rattleAfoxe),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_BIRDS, mx::api::SoundID::rattleBirds),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_CABASA, mx::api::SoundID::rattleCabasa),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_CAXIXI, mx::api::SoundID::rattleCaxixi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_COG, mx::api::SoundID::rattleCog),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_GANZA, mx::api::SoundID::rattleGanza),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_HOSHO, mx::api::SoundID::rattleHosho),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_JAWBONE, mx::api::SoundID::rattleJawbone),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_KAYAMBA, mx::api::SoundID::rattleKayamba),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_KPOKO_KPOKO, mx::api::SoundID::rattleKpokoKpoko),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_LAVA_STONES, mx::api::SoundID::rattleLavaStones),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_MARACA, mx::api::SoundID::rattleMaraca),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_RAIN_STICK, mx::api::SoundID::rattleRainStick),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_RATCHET, mx::api::SoundID::rattleRatchet),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_RATTLE, mx::api::SoundID::rattleRattle),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_SHAKER, mx::api::SoundID::rattleShaker),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_SHAKER_EGG, mx::api::SoundID::rattleShakerEgg),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_SHEKERE, mx::api::SoundID::rattleShekere),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_SISTRE, mx::api::SoundID::rattleSistre),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_TELEVI, mx::api::SoundID::rattleTelevi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_VIBRASLAP, mx::api::SoundID::rattleVibraslap),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::RATTLE_WASEMBE, mx::api::SoundID::rattleWasembe),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_AJAENG, mx::api::SoundID::stringsAjaeng),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_ARPEGGIONE, mx::api::SoundID::stringsArpeggione),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_BARYTON, mx::api::SoundID::stringsBaryton),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_CELLO, mx::api::SoundID::stringsCello),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_CELLO_PICCOLO, mx::api::SoundID::stringsCelloPiccolo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_CONTRABASS, mx::api::SoundID::stringsContrabass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_CRWTH, mx::api::SoundID::stringsCrwth),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_DAN_GAO, mx::api::SoundID::stringsDanGao),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_DIHU, mx::api::SoundID::stringsDihu),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_ERHU, mx::api::SoundID::stringsErhu),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_ERXIAN, mx::api::SoundID::stringsErxian),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_ESRAJ, mx::api::SoundID::stringsEsraj),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_FIDDLE, mx::api::SoundID::stringsFiddle),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_FIDDLE_HARDANGER, mx::api::SoundID::stringsFiddleHardanger),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_GADULKA, mx::api::SoundID::stringsGadulka),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_GAOHU, mx::api::SoundID::stringsGaohu),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_GEHU, mx::api::SoundID::stringsGehu),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_GROUP, mx::api::SoundID::stringsGroup),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_GROUP_SYNTH, mx::api::SoundID::stringsGroupSynth),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_HAEGEUM, mx::api::SoundID::stringsHaegeum),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_HURDY_GURDY, mx::api::SoundID::stringsHurdyGurdy),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_IGIL, mx::api::SoundID::stringsIgil),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_KAMANCHA, mx::api::SoundID::stringsKamancha),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_KOKYU, mx::api::SoundID::stringsKokyu),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_LARUAN, mx::api::SoundID::stringsLaruan),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_LEIQIN, mx::api::SoundID::stringsLeiqin),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_LIRONE, mx::api::SoundID::stringsLirone),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_LYRA_BYZANTINE, mx::api::SoundID::stringsLyraByzantine),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_LYRA_CRETAN, mx::api::SoundID::stringsLyraCretan),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_MORIN_KHUUR, mx::api::SoundID::stringsMorinKhuur),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_NYCKELHARPA, mx::api::SoundID::stringsNyckelharpa),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_OCTOBASS, mx::api::SoundID::stringsOctobass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_REBAB, mx::api::SoundID::stringsRebab),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_REBEC, mx::api::SoundID::stringsRebec),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_SARANGI, mx::api::SoundID::stringsSarangi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_STROH_VIOLIN, mx::api::SoundID::stringsStrohViolin),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_TROMBA_MARINA, mx::api::SoundID::stringsTrombaMarina),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_VIELLE, mx::api::SoundID::stringsVielle),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_VIOL, mx::api::SoundID::stringsViol),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_VIOL_ALTO, mx::api::SoundID::stringsViolAlto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_VIOL_BASS, mx::api::SoundID::stringsViolBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_VIOL_TENOR, mx::api::SoundID::stringsViolTenor),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_VIOL_TREBLE, mx::api::SoundID::stringsViolTreble),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_VIOL_VIOLONE, mx::api::SoundID::stringsViolViolone),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_VIOLA, mx::api::SoundID::stringsViola),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_VIOLA_DAMORE, mx::api::SoundID::stringsViolaDamore),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_VIOLIN, mx::api::SoundID::stringsViolin),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_VIOLONO_PICCOLO, mx::api::SoundID::stringsViolonoPiccolo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_VIOLOTTA, mx::api::SoundID::stringsViolotta),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_YAYLI_TANBUR, mx::api::SoundID::stringsYayliTanbur),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_YAZHENG, mx::api::SoundID::stringsYazheng),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::STRINGS_ZHONGHU, mx::api::SoundID::stringsZhonghu),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_EFFECTS, mx::api::SoundID::synthEffects),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_EFFECTS_ATMOSPHERE, mx::api::SoundID::synthEffectsAtmosphere),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_EFFECTS_BRIGHTNESS, mx::api::SoundID::synthEffectsBrightness),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_EFFECTS_CRYSTAL, mx::api::SoundID::synthEffectsCrystal),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_EFFECTS_ECHOES, mx::api::SoundID::synthEffectsEchoes),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_EFFECTS_GOBLINS, mx::api::SoundID::synthEffectsGoblins),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_EFFECTS_RAIN, mx::api::SoundID::synthEffectsRain),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_EFFECTS_SCI_FI, mx::api::SoundID::synthEffectsSciFi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_EFFECTS_SOUNDTRACK, mx::api::SoundID::synthEffectsSoundtrack),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_GROUP, mx::api::SoundID::synthGroup),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_GROUP_FIFTHS, mx::api::SoundID::synthGroupFifths),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_GROUP_ORCHESTRA, mx::api::SoundID::synthGroupOrchestra),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_PAD, mx::api::SoundID::synthPad),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_PAD_BOWED, mx::api::SoundID::synthPadBowed),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_PAD_CHOIR, mx::api::SoundID::synthPadChoir),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_PAD_HALO, mx::api::SoundID::synthPadHalo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_PAD_METALLIC, mx::api::SoundID::synthPadMetallic),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_PAD_POLYSYNTH, mx::api::SoundID::synthPadPolysynth),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_PAD_SWEEP, mx::api::SoundID::synthPadSweep),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_PAD_WARM, mx::api::SoundID::synthPadWarm),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_THEREMIN, mx::api::SoundID::synthTheremin),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_TONE_SAWTOOTH, mx::api::SoundID::synthToneSawtooth),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_TONE_SINE, mx::api::SoundID::synthToneSine),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::SYNTH_TONE_SQUARE, mx::api::SoundID::synthToneSquare),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_AA, mx::api::SoundID::voiceAa),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_ALTO, mx::api::SoundID::voiceAlto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_AW, mx::api::SoundID::voiceAw),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_BARITONE, mx::api::SoundID::voiceBaritone),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_BASS, mx::api::SoundID::voiceBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_CHILD, mx::api::SoundID::voiceChild),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_COUNTERTENOR, mx::api::SoundID::voiceCountertenor),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_DOO, mx::api::SoundID::voiceDoo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_EE, mx::api::SoundID::voiceEe),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_FEMALE, mx::api::SoundID::voiceFemale),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_KAZOO, mx::api::SoundID::voiceKazoo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_MALE, mx::api::SoundID::voiceMale),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_MEZZO_SOPRANO, mx::api::SoundID::voiceMezzoSoprano),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_MM, mx::api::SoundID::voiceMm),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_OO, mx::api::SoundID::voiceOo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_PERCUSSION, mx::api::SoundID::voicePercussion),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_PERCUSSION_BEATBOX, mx::api::SoundID::voicePercussionBeatbox),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_SOPRANO, mx::api::SoundID::voiceSoprano),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_SYNTH, mx::api::SoundID::voiceSynth),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_TALK_BOX, mx::api::SoundID::voiceTalkBox),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_TENOR, mx::api::SoundID::voiceTenor),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::VOICE_VOCALS, mx::api::SoundID::voiceVocals),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_BANSURI, mx::api::SoundID::windFlutesBansuri),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_BLOWN_BOTTLE, mx::api::SoundID::windFlutesBlownBottle),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_CALLIOPE, mx::api::SoundID::windFlutesCalliope),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_DANSO, mx::api::SoundID::windFlutesDanso),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_DI_ZI, mx::api::SoundID::windFlutesDiZi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_DVOJNICE, mx::api::SoundID::windFlutesDvojnice),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_FIFE, mx::api::SoundID::windFlutesFife),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_FLAGEOLET, mx::api::SoundID::windFlutesFlageolet),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_FLUTE, mx::api::SoundID::windFlutesFlute),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_FLUTE_ALTO, mx::api::SoundID::windFlutesFluteAlto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_FLUTE_BASS, mx::api::SoundID::windFlutesFluteBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_FLUTE_CONTRA_ALTO, mx::api::SoundID::windFlutesFluteContraAlto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_FLUTE_CONTRABASS, mx::api::SoundID::windFlutesFluteContrabass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_FLUTE_DOUBLE_CONTRABASS, mx::api::SoundID::windFlutesFluteDoubleContrabass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_FLUTE_IRISH, mx::api::SoundID::windFlutesFluteIrish),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_FLUTE_PICCOLO, mx::api::SoundID::windFlutesFlutePiccolo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_FLUTE_SUBCONTRABASS, mx::api::SoundID::windFlutesFluteSubcontrabass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_FUJARA, mx::api::SoundID::windFlutesFujara),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_GEMSHORN, mx::api::SoundID::windFlutesGemshorn),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_HOCCHIKU, mx::api::SoundID::windFlutesHocchiku),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_HUN, mx::api::SoundID::windFlutesHun),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_KAVAL, mx::api::SoundID::windFlutesKaval),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_KAWALA, mx::api::SoundID::windFlutesKawala),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_KHLUI, mx::api::SoundID::windFlutesKhlui),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_KNOTWEED, mx::api::SoundID::windFlutesKnotweed),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_KONCOVKA_ALTO, mx::api::SoundID::windFlutesKoncovkaAlto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_KOUDI, mx::api::SoundID::windFlutesKoudi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_NEY, mx::api::SoundID::windFlutesNey),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_NOHKAN, mx::api::SoundID::windFlutesNohkan),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_NOSE, mx::api::SoundID::windFlutesNose),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_OCARINA, mx::api::SoundID::windFlutesOcarina),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_OVERTONE_TENOR, mx::api::SoundID::windFlutesOvertoneTenor),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_PALENDAG, mx::api::SoundID::windFlutesPalendag),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_PANPIPES, mx::api::SoundID::windFlutesPanpipes),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_QUENA, mx::api::SoundID::windFlutesQuena),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_RECORDER, mx::api::SoundID::windFlutesRecorder),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_RECORDER_ALTO, mx::api::SoundID::windFlutesRecorderAlto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_RECORDER_BASS, mx::api::SoundID::windFlutesRecorderBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_RECORDER_CONTRABASS, mx::api::SoundID::windFlutesRecorderContrabass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_RECORDER_DESCANT, mx::api::SoundID::windFlutesRecorderDescant),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_RECORDER_GARKLEIN, mx::api::SoundID::windFlutesRecorderGarklein),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_RECORDER_GREAT_BASS, mx::api::SoundID::windFlutesRecorderGreatBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_RECORDER_SOPRANINO, mx::api::SoundID::windFlutesRecorderSopranino),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_RECORDER_SOPRANO, mx::api::SoundID::windFlutesRecorderSoprano),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_RECORDER_TENOR, mx::api::SoundID::windFlutesRecorderTenor),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_RYUTEKI, mx::api::SoundID::windFlutesRyuteki),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_SHAKUHACHI, mx::api::SoundID::windFlutesShakuhachi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_SHEPHERDS_PIPE, mx::api::SoundID::windFlutesShepherdsPipe),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_SHINOBUE, mx::api::SoundID::windFlutesShinobue),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_SHVI, mx::api::SoundID::windFlutesShvi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_SULING, mx::api::SoundID::windFlutesSuling),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_TARKA, mx::api::SoundID::windFlutesTarka),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_TUMPONG, mx::api::SoundID::windFlutesTumpong),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_VENU, mx::api::SoundID::windFlutesVenu),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_WHISTLE, mx::api::SoundID::windFlutesWhistle),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_WHISTLE_ALTO, mx::api::SoundID::windFlutesWhistleAlto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_WHISTLE_LOW_IRISH, mx::api::SoundID::windFlutesWhistleLowIrish),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_WHISTLE_SHIVA, mx::api::SoundID::windFlutesWhistleShiva),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_WHISTLE_SLIDE, mx::api::SoundID::windFlutesWhistleSlide),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_WHISTLE_TIN, mx::api::SoundID::windFlutesWhistleTin),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_WHISTLE_TIN_BFLAT, mx::api::SoundID::windFlutesWhistleTinBflat),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_WHISTLE_TIN_D, mx::api::SoundID::windFlutesWhistleTinD),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_XIAO, mx::api::SoundID::windFlutesXiao),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_FLUTES_XUN, mx::api::SoundID::windFlutesXun),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_GROUP, mx::api::SoundID::windGroup),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_JUG, mx::api::SoundID::windJug),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_PIPES_BAGPIPES, mx::api::SoundID::windPipesBagpipes),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_PIPES_GAIDA, mx::api::SoundID::windPipesGaida),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_PIPES_HIGHLAND, mx::api::SoundID::windPipesHighland),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_PIPES_UILLEANN, mx::api::SoundID::windPipesUilleann),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_PUNGI, mx::api::SoundID::windPungi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_ALBOGUE, mx::api::SoundID::windReedAlbogue),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_ALBOKA, mx::api::SoundID::windReedAlboka),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_ALGAITA, mx::api::SoundID::windReedAlgaita),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_ARGHUL, mx::api::SoundID::windReedArghul),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_BASSET_HORN, mx::api::SoundID::windReedBassetHorn),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_BASSOON, mx::api::SoundID::windReedBassoon),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_BAWU, mx::api::SoundID::windReedBawu),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_BIFORA, mx::api::SoundID::windReedBifora),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_BOMBARDE, mx::api::SoundID::windReedBombarde),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CHALUMEAU, mx::api::SoundID::windReedChalumeau),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CLARINET, mx::api::SoundID::windReedClarinet),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CLARINET_A, mx::api::SoundID::windReedClarinetA),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CLARINET_ALTO, mx::api::SoundID::windReedClarinetAlto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CLARINET_BASS, mx::api::SoundID::windReedClarinetBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CLARINET_BASSET, mx::api::SoundID::windReedClarinetBasset),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CLARINET_BFLAT, mx::api::SoundID::windReedClarinetBflat),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CLARINET_CONTRA_ALTO, mx::api::SoundID::windReedClarinetContraAlto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CLARINET_CONTRABASS, mx::api::SoundID::windReedClarinetContrabass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CLARINET_EFLAT, mx::api::SoundID::windReedClarinetEflat),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CLARINET_PICCOLO_AFLAT, mx::api::SoundID::windReedClarinetPiccoloAflat),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CLARINETTE_DAMOUR, mx::api::SoundID::windReedClarinetteDamour),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CONTRABASS, mx::api::SoundID::windReedContrabass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CONTRABASSOON, mx::api::SoundID::windReedContrabassoon),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CORNAMUSE, mx::api::SoundID::windReedCornamuse),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CROMORNE, mx::api::SoundID::windReedCromorne),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CRUMHORN, mx::api::SoundID::windReedCrumhorn),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CRUMHORN_ALTO, mx::api::SoundID::windReedCrumhornAlto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CRUMHORN_BASS, mx::api::SoundID::windReedCrumhornBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CRUMHORN_GREAT_BASS, mx::api::SoundID::windReedCrumhornGreatBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CRUMHORN_SOPRANO, mx::api::SoundID::windReedCrumhornSoprano),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_CRUMHORN_TENOR, mx::api::SoundID::windReedCrumhornTenor),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_DIPLE, mx::api::SoundID::windReedDiple),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_DIPLICA, mx::api::SoundID::windReedDiplica),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_DUDUK, mx::api::SoundID::windReedDuduk),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_DULCIAN, mx::api::SoundID::windReedDulcian),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_DULZAINA, mx::api::SoundID::windReedDulzaina),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_ENGLISH_HORN, mx::api::SoundID::windReedEnglishHorn),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_GUANZI, mx::api::SoundID::windReedGuanzi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_HARMONICA, mx::api::SoundID::windReedHarmonica),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_HARMONICA_BASS, mx::api::SoundID::windReedHarmonicaBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_HECKEL_CLARINA, mx::api::SoundID::windReedHeckelClarina),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_HECKELPHONE, mx::api::SoundID::windReedHeckelphone),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_HECKELPHONE_PICCOLO, mx::api::SoundID::windReedHeckelphonePiccolo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_HECKELPHONE_CLARINET, mx::api::SoundID::windReedHeckelphoneClarinet),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_HICHIRIKI, mx::api::SoundID::windReedHichiriki),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_HIRTENSCHALMEI, mx::api::SoundID::windReedHirtenschalmei),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_HNE, mx::api::SoundID::windReedHne),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_HORNPIPE, mx::api::SoundID::windReedHornpipe),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_HOUGUAN, mx::api::SoundID::windReedHouguan),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_HULUSI, mx::api::SoundID::windReedHulusi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_JOGI_BAJA, mx::api::SoundID::windReedJogiBaja),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_KEN_BAU, mx::api::SoundID::windReedKenBau),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_KHAEN_MOUTH_ORGAN, mx::api::SoundID::windReedKhaenMouthOrgan),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_LAUNEDDAS, mx::api::SoundID::windReedLauneddas),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_MAQRUNAH, mx::api::SoundID::windReedMaqrunah),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_MELODICA, mx::api::SoundID::windReedMelodica),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_MIJWIZ, mx::api::SoundID::windReedMijwiz),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_MIZMAR, mx::api::SoundID::windReedMizmar),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_NADASWARAM, mx::api::SoundID::windReedNadaswaram),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_OBOE, mx::api::SoundID::windReedOboe),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_OBOE_BASS, mx::api::SoundID::windReedOboeBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_OBOE_PICCOLO, mx::api::SoundID::windReedOboePiccolo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_OBOE_DA_CACCIA, mx::api::SoundID::windReedOboeDaCaccia),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_OBOE_DAMORE, mx::api::SoundID::windReedOboeDamore),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_OCTAVIN, mx::api::SoundID::windReedOctavin),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_PI, mx::api::SoundID::windReedPi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_PIBGORN, mx::api::SoundID::windReedPibgorn),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_PIRI, mx::api::SoundID::windReedPiri),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_RACKETT, mx::api::SoundID::windReedRackett),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_RAUSCHPFEIFE, mx::api::SoundID::windReedRauschpfeife),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_RHAITA, mx::api::SoundID::windReedRhaita),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_ROTHPHONE, mx::api::SoundID::windReedRothphone),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SARRUSAPHONE, mx::api::SoundID::windReedSarrusaphone),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SAXONETTE, mx::api::SoundID::windReedSaxonette),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SAXOPHONE, mx::api::SoundID::windReedSaxophone),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SAXOPHONE_ALTO, mx::api::SoundID::windReedSaxophoneAlto),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SAXOPHONE_AULOCHROME, mx::api::SoundID::windReedSaxophoneAulochrome),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SAXOPHONE_BARITONE, mx::api::SoundID::windReedSaxophoneBaritone),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SAXOPHONE_BASS, mx::api::SoundID::windReedSaxophoneBass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SAXOPHONE_CONTRABASS, mx::api::SoundID::windReedSaxophoneContrabass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SAXOPHONE_MELODY, mx::api::SoundID::windReedSaxophoneMelody),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SAXOPHONE_MEZZO_SOPRANO, mx::api::SoundID::windReedSaxophoneMezzoSoprano),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SAXOPHONE_SOPRANINO, mx::api::SoundID::windReedSaxophoneSopranino),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SAXOPHONE_SOPRANISSIMO, mx::api::SoundID::windReedSaxophoneSopranissimo),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SAXOPHONE_SOPRANO, mx::api::SoundID::windReedSaxophoneSoprano),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SAXOPHONE_SUBCONTRABASS, mx::api::SoundID::windReedSaxophoneSubcontrabass),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SAXOPHONE_TENOR, mx::api::SoundID::windReedSaxophoneTenor),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SHAWM, mx::api::SoundID::windReedShawm),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SHENAI, mx::api::SoundID::windReedShenai),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SHENG, mx::api::SoundID::windReedSheng),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SIPSI, mx::api::SoundID::windReedSipsi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SOPILA, mx::api::SoundID::windReedSopila),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SORNA, mx::api::SoundID::windReedSorna),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SRALAI, mx::api::SoundID::windReedSralai),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SUONA, mx::api::SoundID::windReedSuona),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_SURNAI, mx::api::SoundID::windReedSurnai),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_TAEPYEONGSO, mx::api::SoundID::windReedTaepyeongso),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_TAROGATO, mx::api::SoundID::windReedTarogato),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_TAROGATO_ANCIENT, mx::api::SoundID::windReedTarogatoAncient),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_TROMPETA_CHINA, mx::api::SoundID::windReedTrompetaChina),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_TUBAX, mx::api::SoundID::windReedTubax),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_XAPHOON, mx::api::SoundID::windReedXaphoon),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_ZHALEIKA, mx::api::SoundID::windReedZhaleika),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_ZURLA, mx::api::SoundID::windReedZurla),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WIND_REED_ZURNA, mx::api::SoundID::windReedZurna),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_AGOGO_BLOCK, mx::api::SoundID::woodAgogoBlock),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_AGUNG_A_TAMLANG, mx::api::SoundID::woodAgungATamlang),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_AHOKO, mx::api::SoundID::woodAhoko),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_BONES, mx::api::SoundID::woodBones),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_CASTANETS, mx::api::SoundID::woodCastanets),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_CLAVES, mx::api::SoundID::woodClaves),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_DRUM_STICKS, mx::api::SoundID::woodDrumSticks),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_GOURD, mx::api::SoundID::woodGourd),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_GRANITE_BLOCK, mx::api::SoundID::woodGraniteBlock),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_GUBAN, mx::api::SoundID::woodGuban),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_GUIRO, mx::api::SoundID::woodGuiro),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_HYOUSHIGI, mx::api::SoundID::woodHyoushigi),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_IPU, mx::api::SoundID::woodIpu),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_JAM_BLOCK, mx::api::SoundID::woodJamBlock),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_KAEKEEKE, mx::api::SoundID::woodKaekeeke),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_KAGUL, mx::api::SoundID::woodKagul),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_KALAAU, mx::api::SoundID::woodKalaau),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_KASHIKLAR, mx::api::SoundID::woodKashiklar),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_KUBING, mx::api::SoundID::woodKubing),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_PAN_CLAPPERS, mx::api::SoundID::woodPanClappers),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_SAND_BLOCK, mx::api::SoundID::woodSandBlock),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_SLAPSTICK, mx::api::SoundID::woodSlapstick),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_STIR_DRUM, mx::api::SoundID::woodStirDrum),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_TEMPLE_BLOCK, mx::api::SoundID::woodTempleBlock),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_TIC_TOC_BLOCK, mx::api::SoundID::woodTicTocBlock),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_TONETANG, mx::api::SoundID::woodTonetang),
    std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::WOOD_WOOD_BLOCK, mx::api::SoundID::woodWoodBlock),
};


const MxLeftBarlineMap MxConverter::sLeftBarlineMap =
{
    std::make_pair<komp::BarlineType, mx::api::BarlineType>(komp::BarlineType::UNKNOWN, mx::api::BarlineType::unsupported),
    std::make_pair<komp::BarlineType, mx::api::BarlineType>(komp::BarlineType::LEFT_HEAVY_LIGHT, mx::api::BarlineType::heavyLight),
    std::make_pair<komp::BarlineType, mx::api::BarlineType>(komp::BarlineType::LEFT_NONE, mx::api::BarlineType::none),
    std::make_pair<komp::BarlineType, mx::api::BarlineType>(komp::BarlineType::LEFT_NORMAL, mx::api::BarlineType::normal),
    std::make_pair<komp::BarlineType, mx::api::BarlineType>(komp::BarlineType::LEFT_REPEAT, mx::api::BarlineType::heavyLight),
    std::make_pair<komp::BarlineType, mx::api::BarlineType>(komp::BarlineType::LEFT_DOUBLE_LIGHT, mx::api::BarlineType::lightLight),
};


const MxRightBarlineMap MxConverter::sRightBarlineMap =
{
    std::make_pair<komp::BarlineType, mx::api::BarlineType>(komp::BarlineType::UNKNOWN, mx::api::BarlineType::unsupported),
    std::make_pair<komp::BarlineType, mx::api::BarlineType>(komp::BarlineType::RIGHT_LIGHT_HEAVY, mx::api::BarlineType::lightHeavy),
    std::make_pair<komp::BarlineType, mx::api::BarlineType>(komp::BarlineType::RIGHT_NONE, mx::api::BarlineType::none),
    std::make_pair<komp::BarlineType, mx::api::BarlineType>(komp::BarlineType::RIGHT_NORMAL, mx::api::BarlineType::normal),
    std::make_pair<komp::BarlineType, mx::api::BarlineType>(komp::BarlineType::RIGHT_REPEAT, mx::api::BarlineType::lightHeavy),
    std::make_pair<komp::BarlineType, mx::api::BarlineType>(komp::BarlineType::RIGHT_DOUBLE_LIGHT, mx::api::BarlineType::lightLight),
};


const MxMarkSet&
MxConverter::getMarks()
{
    return sMarks;
}

komp::MarkType
MxConverter::convertMark( mx::api::MarkType value, mx::api::Placement placement )
{
    auto lambda = [&](const MxMark& mxMark) {
        return mxMark.markType == value && mxMark.placement == placement;
    };
    const auto iter = std::find_if(getMarks().cbegin(), getMarks().cend(), lambda );
    if (iter == getMarks().cend()) {
        return komp::MarkType::NONE;
    }
    return iter->kompMark;
}

mx::api::MarkType
MxConverter::convertMark( komp::MarkType value, mx::api::Placement placement )
{
    auto lambda = [&](const MxMark& mxMark) {
        return mxMark.kompMark == value && mxMark.placement == placement;
    };
    const auto iter = std::find_if(getMarks().cbegin(), getMarks().cend(), lambda );
    if (iter == getMarks().cend()) {
        return mx::api::MarkType::unspecified;
    }
    return iter->markType;
}

mx::api::MarkType
MxConverter::convertMark(komp::MarkType value)
{
    auto result = convertMark(value, mx::api::Placement::unspecified);
    
    if (result != mx::api::MarkType::unspecified) {
        return result;
    }
    
    result = convertMark(value, mx::api::Placement::above);
    
    if (result != mx::api::MarkType::unspecified) {
        return result;
    }
    
    result = convertMark(value, mx::api::Placement::below);
    
    if (result != mx::api::MarkType::unspecified) {
        return result;
    }
    
    return result;
}

komp::MarkType
MxConverter::convertMark( mx::api::MarkType value )
{
    return convertMark(value, mx::api::Placement::unspecified);
}

komp::ArpeggiateType
MxConverter::convertArpeggiate( mx::api::MarkType value )
{
    switch (value)
    {
    case mx::api::MarkType::arpeggiate:
        return komp::ArpeggiateType::STANDARD;
        
    case mx::api::MarkType::arpeggiateDown:
        return komp::ArpeggiateType::DOWN;
        
    case mx::api::MarkType::arpeggiateUp:
        return komp::ArpeggiateType::UP;

    case mx::api::MarkType::nonArpeggiate:
        return komp::ArpeggiateType::NON;

    default:
        return komp::ArpeggiateType::NONE;
    }
}

const MxClefSymbolMap&
MxConverter::getClefs()
{
    return sClefs;
}

ClefSymbolType
MxConverter::convertClefSymbol(mx::api::ClefSymbol value)
{
    return findKompItem(getClefs(), ClefSymbolType::UNKNOWN, value);
}

mx::api::ClefSymbol
MxConverter::convertClefSymbol(ClefSymbolType value)
{
    return findMxItem(getClefs(), mx::api::ClefSymbol::none, value);
}

komp::ClefType
MxConverter::convertClefType(const mx::api::ClefData& clefData)
{
    const auto clefSymbolType = convertClefSymbol(clefData.symbol);
    
    auto lambda =
    [&clefData, &clefSymbolType] (const std::pair<ClefType, ClefInfo>& item) {
        
        if (item.second.clefSymbolInfo.type != clefSymbolType) {
            return false;
        }
        
        if (item.second.delineationLine != clefData.line) {
            return false;
        }
        
        if (item.second.octaveDisplacement != clefData.octaveChange) {
            return false;
        }
        return true;
    };
    
    const auto iter = std::find_if(Clef::ClefInfoTable.cbegin(),
                                   Clef::ClefInfoTable.cend(),
                                   lambda);
    
    if (iter == Clef::ClefInfoTable.cend()) {
        return komp::ClefType::UNKNOWN;
    }
    
    return iter->first;
}

const MxAccidental&
MxConverter::getAccidentals()
{
    return sAccidentals;
}

const komp::AccidentalType
MxConverter::convertAccidental(mx::api::Accidental value)
{
    return findKompItem(getAccidentals(), komp::AccidentalType::NONE, value);
}

const mx::api::Accidental
MxConverter::convertAccidental(komp::AccidentalType value)
{
    return findMxItem(getAccidentals(), mx::api::Accidental::none, value);
}

const MxPitchStepMap&
MxConverter::getPitches()
{
    return sPitches;
}

const PitchStep
MxConverter::convertPitchStep(mx::api::Step value)
{
    return findKompItem(getPitches(), PitchStep::C, value);
}

const mx::api::Step
MxConverter::convertPitchStep(PitchStep value)
{
    return findMxItem(getPitches(), mx::api::Step::c, value);
}

const MxTemporalTypeMap&
MxConverter::getTemporalTypes()
{
    return sTemporalTypes;
}

const TemporalType
MxConverter::convertTemporalType(mx::api::DurationName value)
{
    return findKompItem(getTemporalTypes(), TemporalType::WHOLE, value);
}

const mx::api::DurationName
MxConverter::convertTemporalType(TemporalType value)
{
    return findMxItem(getTemporalTypes(), mx::api::DurationName::unspecified, value);
}

const MxNoteheadTypeMap&
MxConverter::getNoteheadTypes()
{
    return sNoteheadTypes;
}

const NoteheadType
MxConverter::convertNoteheadType(mx::api::Notehead value)
{
    return findKompItem(getNoteheadTypes(), NoteheadType::UNMAPPED, value);
}

const mx::api::Notehead
MxConverter::convertNoteheadType(NoteheadType value)
{
    return findMxItem(getNoteheadTypes(), mx::api::Notehead::none, value);
}

const StemDirection
MxConverter::convertStemDirection(mx::api::Stem value)
{
    switch (value)
    {
    case mx::api::Stem::unspecified:
        return StemDirection::UNSPECIFIED;
        
    case mx::api::Stem::up:
        return StemDirection::UP;
        
    case mx::api::Stem::down:
        return StemDirection::DOWN;
        
    case mx::api::Stem::none:
        return StemDirection::NONE;
        
    case mx::api::Stem::both:
        return StemDirection::BOTH;
    }
    
    return StemDirection::UNSPECIFIED;
}

const mx::api::Stem
MxConverter::convertStemDirection(StemDirection value)
{
    switch (value)
    {
    case StemDirection::UNSPECIFIED:
        return mx::api::Stem::unspecified;
        
    case StemDirection::UP:
        return mx::api::Stem::up;
        
    case StemDirection::DOWN:
        return mx::api::Stem::down;
        
    case StemDirection::NONE:
        return mx::api::Stem::none;
        
    case StemDirection::BOTH:
        return mx::api::Stem::both;
    }
    
    return mx::api::Stem::unspecified;
}

const MxTimeSignatureTypeMap&
MxConverter::getTimeSignatureTypes()
{
    return sTimeSignatureTypes;
}

const TimeSignatureType
MxConverter::convertTimeSignatureType(mx::api::TimeSignatureSymbol value)
{
    return findKompItem(getTimeSignatureTypes(), TimeSignatureType::NUMERIC, value);
}

const mx::api::TimeSignatureSymbol
MxConverter::convertTimeSignatureType(TimeSignatureType value)
{
    return findMxItem(getTimeSignatureTypes(), mx::api::TimeSignatureSymbol::unspecified, value);
}

KeySignatureValue
MxConverter::convertKey(const mx::api::KeyData& inKey)
{
    auto accidentals = inKey.fifths;
    if (accidentals < -7) {
        accidentals = -7;
    } else if (accidentals > 7) {
        accidentals = 7;
    }
    
    KeySignatureValue keyValue;
    keyValue.setAccidentals(accidentals);
    return keyValue;
}

mx::api::KeyData
MxConverter::convertKey(const KeySignatureValue& inKey)
{
    mx::api::KeyData outKey;
    outKey.mode = mx::api::KeyMode::unspecified;
    outKey.fifths = inKey.getAccidentals();
    return outKey;
}

const MxOttavaMap&
MxConverter::getOttavas()
{
    return sOttavaMap;
}

const OctaveType
MxConverter::convertOttava(mx::api::OttavaType value)
{
    return findKompItem(getOttavas(), OctaveType::UNKNOWN, value);
}

const mx::api::OttavaType
MxConverter::convertOttava(OctaveType value)
{
    return findMxItem(getOttavas(), mx::api::OttavaType::unspecified, value);
}

const MxBeamChunkMap&
MxConverter::getBeamChunkMap()
{
    return sBeamChunkMap;
}

const BeamChunk
MxConverter::convertBeamChunk(mx::api::Beam value)
{
    return findKompItem(getBeamChunkMap(), BeamChunk::Unknown, value);
}

const mx::api::Beam
MxConverter::convertBeamChunk(BeamChunk value)
{
    return findMxItem(getBeamChunkMap(), mx::api::Beam::unspecified, value);
}

const MxSoundMap&
MxConverter::getSoundMap()
{
    return sSoundMap;
}

const komp::SoundID
MxConverter::convertSoundID(mx::api::SoundID value)
{
    return findKompItem(getSoundMap(), komp::SoundID::KEYBOARD_PIANO, value);
}


const mx::api::SoundID
MxConverter::convertSoundID(komp::SoundID value)
{
    return findMxItem(getSoundMap(), mx::api::SoundID::unspecified, value);
}


const MxLeftBarlineMap&
MxConverter::getLeftBarlineMap()
{
    return sLeftBarlineMap;
}


const komp::BarlineType
MxConverter::convertLeftBarline(mx::api::BarlineType inValue)
{
    return findKompItem(getLeftBarlineMap(), komp::BarlineType::UNKNOWN, inValue);
}


const mx::api::BarlineType
MxConverter::convertLeftBarline(komp::BarlineType inValue)
{
    return findMxItem(getLeftBarlineMap(), mx::api::BarlineType::unsupported, inValue);
}


const MxRightBarlineMap&
MxConverter::getRightBarlineMap()
{
    return sRightBarlineMap;
}


const komp::BarlineType
MxConverter::convertRightBarline(mx::api::BarlineType inValue)
{
    return findKompItem(getRightBarlineMap(), komp::BarlineType::UNKNOWN, inValue);
}


const mx::api::BarlineType
MxConverter::convertRightBarline(komp::BarlineType inValue)
{
    return findMxItem(getRightBarlineMap(), mx::api::BarlineType::unsupported, inValue);
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

