// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "Attachments.h"
#include "Duration.h"
#include "Item.h"
#include "MarkType.h"
#include "NoteFrames.h"
#include "PlaybackNote.h"
#include "StemInfo.h"
#include "StemPolicy.h"
#include "StemType.h"

namespace komp
{
    /// \brief  Temporals represent Notes and Rests in the Score
    ///
    /// \discussion Notes, Rests and Chords are all children of Temporal.
    /// There is some questionable Liskov substitution between the sub-types
    /// of Temporal.  For example a Note has a pitch, a Rest does not, and
    /// a Chord has multiple pitches.  Nonetheless, a Temporal has a getPitch()
    /// function which is only meaningful for Notes.  It was decided in February
    /// 2017 that the Liskov substitution issues were less harmful than frequent
    /// downcasting into the subtypes.  Many of the functions that had
    /// previously only been available on the subtypes were pushed up to the
    /// parent.  In order to further prevent downcasting of Temporals, the
    /// NoteType enum was added so that clients can ask the parent Temporal if
    /// it is a Note, Rest or Chord without including those header files.
    ///
    class KOMP_EXPORT Temporal : public Item
    {
    public:
        virtual ~Temporal() = default;

        ///{@ Item
        virtual const ItemTraits& getItemTraits() const override = 0;
        virtual ItemUPtr getMemento() const override = 0;
        virtual void restoreMemento(const Item& inItem) override = 0;
        virtual void itemIDChanged() override = 0;
        virtual void itemIDInvalidated() override = 0;
        virtual void registerObserver(ItemObserver& inObserver) override = 0;
        virtual void unregisterObserver(ItemObserver& inObserver) override = 0;
        virtual void notifyObservers(const ItemChanges& inChanges) override = 0;
        virtual void observedItemChanged(const Item& observedItem, const ItemChanges& inChanges) override = 0;
        virtual bool getIsQuiescent() const override = 0;
        virtual void setIsQuiescent(bool inValue) override = 0;
        virtual Rect getFrame() const override = 0;
        virtual Region getRegion() const override = 0;
        virtual Point getAnchor() const override = 0;
        virtual bool contains(const Point& inPoint, bool inPrecise) const override = 0;
        virtual void inserted() override = 0;
        virtual void removed() override = 0;
        virtual void removeSelf() override = 0;
        virtual const MusicLocation& getLocation() const override = 0;
        virtual double getX() const override = 0;
        virtual void setX(double inValue) override = 0;
        virtual double getScoreX() const override = 0;
        virtual int getSlot() const override = 0;
        virtual void setSlot(int inValue) override = 0;
        virtual double getScoreY() const override = 0;
        virtual Point getDrawPoint() const override = 0;
        virtual double getTime() const override = 0;
        virtual int getRoundedTime() const override = 0;
        virtual void setTime(double inValue) override = 0;
        virtual double getSpaceSize() const override = 0;
        virtual double getFontSize() const override = 0;
        ///@}

        /// is this temporal is a Note
        virtual bool getIsNote() const = 0;

        /// is this temporal is a Rest
        virtual bool getIsRest() const = 0;

        /// is this temporal is a Chord
        virtual bool getIsChord() const = 0;

        /// returns true if the Temporal is the member of a chord, i.e. the
        /// Temporal is owned and controlled by a Chord object.  A Note will
        /// return true if it is a member of a Chord. A Rest will typically
        /// return false, though there is no outright prohibition of a Rest
        /// becoming a member of a Chord. A Chord will always return false
        /// because a Chord cannot become a member of another Chord.
        virtual bool getIsChordMember() const = 0;

        /// returns the Chord object if the Temporal is owned by a Chord.
        /// A Chord object will always return false because a Chord cannot
        /// be owned by another Chord.
        virtual Chord* getChord() const = 0;

        /// sets the chord object. clients generally should not use this and should rely
        /// instead on the Chord object to set this, or rely on a CommandIntf object.
        virtual void setChord(Chord* inChord) = 0;

        /// Clears the chord object. clients generally should not use this and should rely
        /// instead on the Chord object to set this, or rely on a CommandIntf object.
        virtual void clearChord() = 0;

        /// returns true if this temporal should be considered during
        /// the application of note-spacing or justification routines
        virtual bool getIsLayoutItem() const = 0;

        /// return the pitch of the temporal.  If the temporal is silent
        /// (such as a rest or a tied-to note) then the value of getPitch()
        /// is not useful.  In the case of a Chord. It is only relevant for
        /// non-silent notes.
        virtual Pitch getPitch() const = 0;

        /// get the primary glyph that represents the temporal
        virtual unsigned short getGlyph() const = 0;
        
        /// get the duration
        virtual Duration getDuration() const = 0;

        /// set the duration
        virtual void setDuration(Duration inValue) = 0;

        /// a raw note is a stemless quarter note head that has been
        /// entered by the user, but not yet been assigned a duration
        /// by the user (because it has no stem or flags).
        virtual bool getIsRaw() const = 0;

        /// see getIsRaw() for description
        virtual void setIsRaw(bool inValue) = 0;

        /// set the stem direction and tell the Temporal whether further
        /// updates to the stem direction should be done automatically.
        /// note that getting and setting the stem-policy will be passed
        /// through to the stem owner in the case that this Temporal is
        /// not the owner.  For example, if this Temporal is a Note, and
        /// if the Note has a Beam, the ownership of the Stem and policy
        /// will be defered to the Beam.  getStemInfo().isStemOwner to find out.
        virtual StemPolicy getStemPolicy() const = 0;
        virtual void setStemPolicy(StemPolicy inPolicy) = 0;

        /// returns information useful when building up a stem
        /// for a temporal. caution, these are not the actual
        /// values, but rather the guidance given by the temporal
        /// to any client who wishes to calculate and draw a stem
        virtual StemInfo getStemInfo() const = 0;

        /// gets information about how to playback the temporal.
        virtual PlaybackNote getPlaybackNote() const = 0;

        /// get the Curves and Marks container
        virtual Attachments& getAttachments() = 0;

        /// get the Curves and Marks container
        virtual const Attachments& getAttachments() const = 0;

        /// is the temporal part of a beam
        virtual bool getIsBeamed() const = 0;

        /// get the beam that the temporal is a part of. can be null
        virtual BeamIntf* getBeam() const = 0;

        /// Remove the beam that the temporal is a part of.
        virtual void clearBeam() = 0;

        /// set the beam that the temporal is a part of. clients should not
        /// use this. instead this should be set by the Beam object
        virtual void setBeam(BeamIntf& inBeam) = 0;

        /// returns true if this temporal is affected by one or more tuplets
        virtual bool getIsTupleted() const = 0;

        /// returns this temporal's tuplets
        virtual TupletIntfRefVec getTuplets() const = 0;

        /// adds a tuplet to the list of tuplets modifying this temporal.  nested tuplets
        /// are expected to added in the order of innermost-first, outtermost-last and the
        /// internal vector will be assumed to represent this correctly. returns true if the
        /// tuplet was added, returns false if the tuplet was duplicative/not-added.  Adding
        /// a tuplet will also have the side-effect of altering this temporals Duration to
        /// accound for the tuplet.
        virtual bool addTuplet(TupletIntf& inTuplet) = 0;

        /// removes the tuplet from this temporals collection. returns true if the tuplet
        /// was found and removed from the collection.  Removing a tuplet will have the
        /// side-effect of remove the tuplet's modification ratio from the Duration
        virtual bool removeTuplet(TupletIntf& inTuplet) = 0;

        /// returns a frame for each of the constituent parts of the note, rest, or
        /// chord.  if a particular part of does not exist, then the frame for that
        /// part will be a zero rect. For example a whole note has stem rect zero.
        virtual NoteFrames getFrames() const = 0;

        /// returns true if this is the first non-chord temporal in a tuplet (for MusicXML export)
        virtual bool getIsTupletStart() const = 0;

        /// returns true if this is the last non-chord temporal in a tuplet (for MusicXML export)
        virtual bool getIsTupletStop() const = 0;
        
        virtual bool getIsGrace() const = 0;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
