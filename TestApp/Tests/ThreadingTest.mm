// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// Komp
#include "DrawableContext.h"
#include "Duration.h"
#include "Measure.h"
#include "Staff.h"
#include "UndoManager.h"

// Test Support
#include "BasicScore.h"
#include "TestNoteFactory.h"
#include "TupletScores.h"
#include "TestFileRepository.h"

#include <thread>

#include <CoreGraphics/CoreGraphics.h>
#include <Cocoa/Cocoa.h>

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS

    TEST( torture, ThreadTest )
    {
        bool isTestSuccessful = false;

        NSRect imgRect = NSMakeRect(0.0, 0.0, 1000.0, 1000.0);
        NSSize imgSize = imgRect.size;

        NSBitmapImageRep *offscreenRep = [[NSBitmapImageRep alloc]
                                           initWithBitmapDataPlanes:NULL
                                           pixelsWide:imgSize.width
                                           pixelsHigh:imgSize.height
                                           bitsPerSample:8
                                           samplesPerPixel:4
                                           hasAlpha:YES
                                           isPlanar:NO
                                           colorSpaceName:NSDeviceRGBColorSpace
                                           bitmapFormat:NSBitmapFormatAlphaFirst
                                           bytesPerRow:0
                                           bitsPerPixel:0];

        // set offscreen context
        NSGraphicsContext *g = [NSGraphicsContext graphicsContextWithBitmapImageRep:offscreenRep];
        [NSGraphicsContext saveGraphicsState];
        [NSGraphicsContext setCurrentContext:g];

        // draw first stroke with Cocoa
//        NSPoint p1 = NSMakePoint(NSMaxX(imgRect), NSMinY(imgRect));
//        NSPoint p2 = NSMakePoint(NSMinX(imgRect), NSMaxY(imgRect));
//        [NSBezierPath strokeLineFromPoint:p1 toPoint:p2];

        // draw second stroke with Core Graphics
//        CGContextRef ctx = g.CGContext;
//        CGContextBeginPath(ctx);
//        CGContextMoveToPoint(ctx, 0.0, 0.0);
//        CGContextAddLineToPoint(ctx, imgSize.width, imgSize.height);
//        CGContextClosePath(ctx);
//        CGContextStrokePath(ctx);

        // done drawing, so set the current context back to what it was
        [NSGraphicsContext restoreGraphicsState];
        
        // create an NSImage and add the rep to it    
        NSImage *img = [[NSImage alloc] initWithSize:imgSize];
        [img addRepresentation:offscreenRep];
        
        // TODO: Save or view the NSImage

        const auto score = getBasicScore( 2, 4 );
        const int spinCount = 800;
        const int noteCount = 10;
        
        auto& reg = score->getItemRegistry();
        const MusicLocation loc(*score->getMeasure(0).get(),
                                *score->getTracks().at(0).get());

        auto inserts = [&]() {
            for (int i = 0; i < spinCount; ++i) {
                for (int j = 0; j < noteCount; ++j) {
                    const auto& note = reg.createItem<Note>(loc,
                                                            TemporalType::EIGHTH,
                                                            NoteheadType::NORMAL,
                                                            Pitch{});
                    auto insertCmd = score->getCommandFactory().insertTemporalCommand(note.getItemID(),
                                                                                      loc.getMeasure());
                    insertCmd->execute();
                    score->getUndoManager().addCommand(std::move(insertCmd));
                }
                for (int j = 0; j < noteCount; ++j) {
                    score->getUndoManager().undo();
                }
            }
            std::cout << "Threading Test inserts complete" << std::endl;
        };

        auto draws = [&]() {
            for (int i = 0; i < spinCount * 2; ++i) {
                // FIXME: Score cannot be drawn on a thread
                //score->draw();
            }
            std::cout << "Threading Test drawing complete" << std::endl;
        };

        std::thread insertThread{inserts};
        std::thread drawThread{draws};
        insertThread.join();
        drawThread.join();

        isTestSuccessful = true;
        CHECK( isTestSuccessful );
    }
    T_END;

#endif
};

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
