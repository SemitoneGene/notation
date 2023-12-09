// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "ArpeggiateType.h"
#include "ChordNote.h"
#include "Geometry.h"
#include "SpaceTimePoint.h"
#include "Temporal.h"

namespace komp
{
struct ChordMemento
{
    ItemUPtrVec noteMementos;
    ItemUPtr chordMemento;
};

class KOMP_EXPORT Chord : public Temporal
{
public:
    virtual ~Chord() = default;
    
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
    virtual void setBeam(BeamIntf& inBeam) override;
    virtual void clearBeam() override;
    virtual bool getIsTupleted() const override;
    virtual TupletIntfRefVec getTuplets() const override;
    virtual bool addTuplet(TupletIntf& inTuplet) override;
    virtual bool removeTuplet(TupletIntf& inTuplet) override;
    virtual NoteFrames getFrames() const override;
    virtual bool getIsTupletStart() const override;
    virtual bool getIsTupletStop() const override;
    virtual bool getIsGrace()  const override;
    ///@}
    
    ///@{ Chord //////////////////////////////////////////////////
    
    /// Returns references to this chords note members, sorted by
    /// slot ascending, then x ascending
    std::vector<NoteRef> getNotes() const;
    
    /// Returns true if the note was added.  if notes already exist
    /// in the chord, then the inNote will be altered (if necessary)
    /// so that it's duration matches the existing notes in the chord.
    bool addNote(Note& inNote);
    
    /// Removes all notes from the chord
    void clearNotes();
    
    /// Removes the note from the chord, true if a note was removed
    bool removeNote(const Note& inNote);
    
    /// Returns the note with the highest slot. if there are more
    /// than one at the highest slot then returns the one with the
    /// highest x value
    Note& getTopNote() const;
    
    /// Returns the note with the lowest slot. if there are more
    /// than one at the lowest slot then returns the one with the
    /// lowest x value
    Note& getBottomNote() const;
    
    /// Returns true if the inNote is a member of this chord
    bool exists(const Note& inNote) const;
    
    /// Recalculate the positioning of noteheads, accidentals and dots
    void updateChordLayout();
    
    /// Returns false if all the noteheads are aligned with the same x
    /// otherwise, returns true if one or more noteheads is flipped
    /// out to the right to accommodate unisons or seconds
    bool getIsAnyNoteFlipped() const;
    
    /// Returns a frame around the stem
    Rect getStemFrame() const;
    
    /// Return a memento
    ChordMemento getChordMemento() const;
    
    // Returns true if the passed note is contained in the chord
    bool getIsMember(const Note& inNote) const;
    
    // Returns true if the passed note is the first (i.e. bottom)
    // note of the chord
    bool getIsFirst(const Note& inNote) const;
    
    // Returns true if the passed note is the last (i.e. top)
    // note of the chord
    bool getIsLast(const Note& inNote) const;
    
    /// Get the temporal's arpeggiate
    ArpeggiateType getArpeggiate() const;
    
    /// Set the temporal's arpeggiate
    void setArpeggiate(ArpeggiateType inArpeggiate);
    
    ///@} Chord //////////////////////////////////////////////////
    
private:
    friend class ItemRegistry;
    Chord(const MusicLocation& inLocation, const NoteRefVec& inNotes);
    
    virtual void createDrawable() override;
    
private:
    
    /// set each of the notes' chord property to 'this'
    void registerNotes();
    
    /// set each of the notes' chord property to nullptr
    void unregisterNotes();
    
    double getMeasureDataX() const;
    Note* findNote(const Item& inItem) const;
    void observeNotes();
    void unobserveNotes();
    void updateInternal();
    void updateDurations();
    void updateStemPolicy(ChordHeadPtrs& ioHeads);
    void updateAccidentals();
    void updateArpeggiate();
    void updateHorizonalPositions();
    void sort();
    bool addFirstNote(const Note& inNote);
    bool addAdditionalNote(Note& inNote);
    void updateDrawables();
    void completeConstruction();
    bool addNoteImpl(Note& inNote);
    ChordHeadPtrs getChordHeadPtrStructs(ChordNotes& chordNotes) const;
    
    void updateBeamPassOne();
    void updateBeamPassTwo();
    void updateBeam();
    
    void migrateAttachments();
    
    Attachments mAttachments;
    BeamIntf* mBeam{nullptr};
    bool mIsObservingNotes{false};  ///< True if the chord has registered itself as an observer of its notes
    bool mIsRaw{false};
    bool mIsRegistered{false};
    double mStemRise{3.5};          ///< The protrusion length of the stem in staff spaces
    double mStemX{0.0};             ///< Stem location in pixels relative to chord->getX()
    Duration mDuration{};
    SpaceTimePoint mSpaceTimePoint;
    ChordNotes mNotes{};
    ItemObserverRefVec mObservers{};
    int mObserverSpinCount{0};
    StemPolicy mStemPolicy{};
    TupletIntfRefVec mTuplets{};
    bool mIsInserted{false};
    NoteRefVec mConstructorNotes;
    bool mIsQuiescent{true};
    MusicLocation mLocation;
    ArpeggiateType mArpeggiateType{ArpeggiateType::NONE};
    ItemChanges mDeferredChanges;
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
