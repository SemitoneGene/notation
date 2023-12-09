// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "NoteheadType.h"
#include "SpaceTimePoint.h"
#include "Temporal.h"

namespace komp
{
    class KOMP_EXPORT Note : public Temporal
    {
    public:
        virtual ~Note() = default;

        ///{@ Item
        virtual const ItemTraits& getItemTraits() const override;
        virtual ItemUPtr getMemento() const override;
        virtual void restoreMemento(const Item& inItem) override;
        virtual void update() override;
        virtual void itemIDChanged() override;
        virtual void itemIDInvalidated() override;
        virtual void registerObserver(ItemObserver& inObserver) override;
        virtual void unregisterObserver(ItemObserver& inObserver) override;
        virtual void notifyObservers(const ItemChanges& inChanges) override;
        virtual void observedItemChanged(const Item& observedItem, const ItemChanges& inChanges) override;
        virtual bool getIsQuiescent() const override;
        virtual void setIsQuiescent(bool inValue) override;
        virtual Rect getFrame() const override;
        virtual Region getRegion() const override;
        virtual Point getAnchor() const override;
        virtual bool contains(const Point& inPoint, bool inPrecise) const override;
        virtual void inserted() override;
        virtual void removed()override;
        virtual void removeSelf() override;
        virtual const MusicLocation& getLocation() const override;
        virtual double getX() const override;
        virtual void setX(double inValue) override;
        virtual double getScoreX() const override;
        virtual int getSlot() const override;
        virtual void setSlot(int inValue) override;
        virtual double getScoreY() const override;
        virtual double getTime() const override;
        virtual int getRoundedTime() const override;
        virtual void setTime(double inValue) override;
        virtual double getSpaceSize() const override;
        virtual double getFontSize() const override;
        virtual Point getDrawPoint() const override;
        ///@}

        ///@{ Temporal
        virtual bool getIsNote() const override;
        virtual bool getIsRest() const override;
        virtual bool getIsChord() const override;
        virtual bool getIsChordMember() const override;
        virtual Chord* getChord() const override;
        virtual void setChord(Chord* inChord) override;
        virtual void clearChord() override;
        virtual bool getIsLayoutItem() const override;
        virtual Pitch getPitch() const override;
        virtual unsigned short getGlyph() const override;
        virtual Duration getDuration() const override;
        virtual void setDuration(Duration inDuration) override;
        virtual bool getIsRaw() const override;
        virtual void setIsRaw(bool inValue) override;
        virtual StemPolicy getStemPolicy() const override;
        virtual void setStemPolicy(StemPolicy inPolicy) override;
        virtual StemInfo getStemInfo() const override;
        virtual PlaybackNote getPlaybackNote() const override;
        virtual Attachments& getAttachments() override;
        virtual const Attachments& getAttachments() const override;
        virtual bool getIsBeamed() const override;
        virtual BeamIntf* getBeam() const override;
        virtual void clearBeam() override;
        virtual void setBeam(BeamIntf& inBeam) override;
        virtual bool getIsTupleted() const override;
        virtual TupletIntfRefVec getTuplets() const override;
        virtual bool addTuplet(TupletIntf& inTuplet) override;
        virtual bool removeTuplet(TupletIntf& inTuplet) override;
        virtual NoteFrames getFrames() const override;
        virtual bool getIsTupletStart() const override;
        virtual bool getIsTupletStop() const override;
        virtual bool getIsGrace() const override;
        ///@}

        /// Get the temporal's accidental
        AccidentalType getAccidental() const;

        /// Set the temporal's accidental
        void setAccidental(AccidentalType inAccidental);

        /// Get the x-offset, in pixels, between the accidental's
        /// draw point and the temporal's draw point
        double getAccidentalOffset() const;

        /// Set the x-offset, in pixels, between the accidental's
        /// draw point and the temporal's draw point
        void setAccidentalOffset(double inValue);

        /// Resets the x-offset of the accidental to default positioning
        void setAccidentalOffsetDefault();

        /// \Given the current pitch, and clef, this will
        /// move the note to the correct slot.
        void deduceSlot();

        /// Given the current pitch, clef, slot, and measure
        /// accidental history will alter the note using the
        /// correct accidental
        void deduceAccidental();

        /// \discussion Calculates and sets the midi pitch by parsing clef, key, slot and
        /// measure accidental history. Use this function after changing the
        /// state of a Temporal or a measure to reconcile the Temporal's midi
        /// pitch with its current pitch-related state and context. This will
        /// throw an exception if note is not in a Measure.  This function
        /// works on Rests even though the assignment of a Pitch in a Rest is
        /// not particularly meaningful.  If called on a Chord, the Chord will
        /// pass-through the call to all of its members.
        void deducePitch();
        
        NoteheadType getNoteheadType() const { return mNoteheadType; }
        void setNoteheadType(NoteheadType inNotehead) { mNoteheadType = inNotehead; }
                
        /// \discussion Mark placement cannot be handled internal to the marks themselves. 
        void placeMarks();

    private:
        Note() = delete;

        Note(const MusicLocation& inMusicLocation,
             TemporalType inType,
             NoteheadType inNoteheadType = NoteheadType::NORMAL,
             Pitch inPitch = Pitch{},
             bool isGrace = false);

        virtual void beginDrag(const CurrentState& inState) override;
        virtual void drag(const CurrentState& inState) override;
        virtual void endDrag(const CurrentState& inState) override;
        
        void updateStemDirection();
        void updateBeam();
        void updateChord();
        void updateTuplet();

    private:
        friend class ItemRegistry;
        
        virtual void createDrawable() override;

        Attachments mAttachments;
        BeamIntf* mBeam{nullptr};
        bool mIsRaw{false};
        bool mIsRegistered{false};
        Chord* mChord{nullptr};
        Duration mDuration;
        Pitch mPitch;
        NoteheadType mNoteheadType;
        SpaceTimePoint mSpaceTimePoint;
        ItemObserverRefVec mObservers;
        StemPolicy mStemPolicy;
        TupletIntfRefVec mTuplets;
        Point mDragStartOffset;
        int mObserverSpinCount{0};
        bool mIsQuiescent{true};
        bool mIsGrace;
        ItemChanges mDeferredChanges;
        MusicLocation mLocation;
        AccidentalType mAccidentalType{AccidentalType::NONE};
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
