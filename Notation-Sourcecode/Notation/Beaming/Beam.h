// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "BeamIntf.h"
#include "BeamSpecIntf.h"
#include "DrawableComposite.h"

namespace komp
{
    class Beam : public BeamIntf
    {
    public:
        Beam(const Beam& other);
        Beam& operator=(const Beam& other);

        ///@{ Item
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
        virtual void notifyMeasureXChanged() override;
        virtual void notifyStaffYChanged() override;
        virtual void beginDrag(const CurrentState& inState) override;
        virtual void drag(const CurrentState& inState) override;
        virtual void endDrag(const CurrentState& inState) override;
        ///@}
        
        ///@{ BeamIntf
        virtual Rect getBeamLinesFrame() const override;
        // TODO: updateBeam() why have callers use a special function for beam updates instead of item->update()?
        virtual void updateBeam() override;
        virtual const BeamedNoteIntfUPtrVec& getBeamedNotes() const override;
        virtual TemporalRefVec getNotes() const override;
        virtual StemPolicy getStemPolicy() const override;
        virtual void setStemPolicy(StemPolicy inPolicy) override;
        virtual bool getIsFlat() const override;
        virtual double getYIntercept(double inScoreX) const override;
        virtual double getBeamLineThickness(int inBeamLineIndex) const override;
        virtual Rect getStemFrame(const Temporal& inTemporal) const override;
        virtual bool getIsMember(const Temporal& inTemporal) const override;
        virtual bool getIsFirst(const Temporal& inTemporal) const override;
        virtual bool getIsLast(const Temporal& inTemporal) const override;
        ///@}

        ///@{ Local
        const BeamSpecIntf& getBeamSpec() const;
        ///@}

    private:

        struct LineCache
        {
            double measureRelativeX1;
            double measureRelativeX2;
            double slot1;
            double slot2;
            double thickness;
            double slope;
            
            LineCache()
            : measureRelativeX1{0}
            , measureRelativeX2{0}
            , slot1{0}
            , slot2{0}
            , thickness{0}
            , slope{0}
            {

            }
        };

        struct StemCache
        {
            TemporalRef temporal;
            LineCache stemLine;

            StemCache() = delete;

            StemCache(Temporal& inTemporal, LineCache&& inLineCache)
            : temporal{inTemporal}
            , stemLine{std::move(inLineCache)}
            {
                stemLine.slope = INFINITY;
            }
        };

        struct Cache
        {
            bool isDirty;
            std::vector<LineCache> beamLines;
            inline LineCache* getMainBeam() { return beamLines.size() > 0 ? &beamLines.front() : nullptr; }
            double mainBeamStartY;
            double measureDataX;
            StemPolicy stemPolicy;
            bool isStemDirty;
            std::vector<StemCache> stems;
            Rect frame;
            Region region;
            bool isFrameDirty;

            Cache()
            : isDirty(true)
            , beamLines()
            , mainBeamStartY(0.0)
            , measureDataX(0.0)
            , stemPolicy{StemDirection::UP, Policy::AUTOMATIC}
            , isStemDirty{true}
            , stems{}
            , frame{}
            , region{}
            , isFrameDirty{true}
            {
                
            }
        };

    private:
        friend class ItemRegistry;
        
        virtual void createDrawable() override;
        
        Beam(const MusicLocation& inLocation,
                const BeamSpecIntf& inBeamSpec);
        void copyState(const Beam& other);
        void updateBeamSpec() const;
        void notifyNotes();
        void notifyNotesRemoved();
        void observe();
        void unobserve();
        void clearCache() const;
        void cacheBeamLineValues() const;
        void cacheStemValues() const;
        void cacheFrame() const;
        void updateDrawables() const;

        ItemObserverRefVec mObservers;
        bool mIsQuiescent{true};
        BeamSpecIntfUPtr mBeamSpec;
        bool mAreNotesNotified{false};
        mutable Cache mCache;
        bool mHasStemFlipEventOccurred{false};
        MusicLocation mLocation;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
