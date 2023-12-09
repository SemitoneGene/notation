// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "DrawableRestParts.h"
#include "SpaceTimePoint.h"
#include "Temporal.h"

namespace komp
{
    class KOMP_EXPORT Rest : public Temporal
    {
    public:
        virtual ~Rest() {}

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
        virtual void removed() override;
        virtual void removeSelf() override;
        virtual const MusicLocation& getLocation() const override;
        virtual double getX() const override;
        virtual void setX(double inValue) override;
        virtual double getScoreX() const override;
        virtual int getSlot() const override;
        virtual void setSlot(int inValue) override;
        virtual double getScoreY() const override;
        virtual Point getDrawPoint() const override;
        virtual double getTime() const override;
        virtual int getRoundedTime() const override;
        virtual void setTime(double inValue) override;
        virtual double getSpaceSize() const override;
        virtual double getFontSize() const override;
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
        virtual void setDuration(Duration inValue) override;
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
        virtual bool getIsGrace()  const override;
        ///@}
        
        ///@{ Draggable
        virtual void drag(const CurrentState& inState) override;
        ///@}

        int slotForType() const;
        int slotForVoice(int inTotalVoices) const;

    private:
        friend class ItemRegistry;
        
        Rest() = delete;
                
        Rest(const MusicLocation& inMusicLocation,
             TemporalType inTemporalType);
        
        ///@{ Item
        virtual void createDrawable() override;
        ///@}

        void drawSlash(const DrawableContext& inContext,
                       Staff* inStaff,
                       double inX);
        void drawCompressed(const DrawableContext& inContext, Staff* inStaff);
        
    private:
        Attachments mAttachments;
        BeamIntf* mBeam;
        bool mIsRaw;
        bool mIsRegistered;
        Chord* mChord;
        Duration mDuration;
        Pitch mPitch;
        SpaceTimePoint mSpaceTimePoint;
        ItemObserverRefVec mObservers;
        StemPolicy mStemPolicy;
        TupletIntfRefVec mTuplets;
        int mObserverSpinCount;
        bool mIsQuiescent;
        MusicLocation mLocation;
        ItemChanges mDeferredChanges;
    };
        
} // namespace


// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
