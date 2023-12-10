// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "Cpul/cpulTest.h"

// Komp
#include "DrawableContext.h"
#include "FileMediatorFactory.h"
#include "TestSupport/TestFileRepository.h"
#include "Score.h"


#include <CoreGraphics/CoreGraphics.h>

#include <string>

namespace
{
    // RAII for the CGContext
    struct ContextHolder
    {
        CGContextRef context;
        CGColorSpaceRef colorSpace;
        void* bitmapData;

        ContextHolder()
        : context(NULL)
        , colorSpace(NULL)
        , bitmapData(NULL)
        {

        }

        ContextHolder(const ContextHolder& other) = delete;
        ContextHolder& operator=(const ContextHolder& other) = delete;
        ContextHolder(ContextHolder&& other) = default;
        ContextHolder& operator=(ContextHolder&& other) = default;

        ~ContextHolder() { CGContextRelease(context); }

        static void releaseCallback(void* releaseInfo, void* instance)
        {
            ContextHolder* myself = reinterpret_cast<ContextHolder*>(instance);
            CGColorSpaceRelease(myself->colorSpace);
            free(myself->bitmapData);
            myself->bitmapData = nullptr;
            myself->colorSpace = nullptr;
        }
    };
}

namespace komp
{
    class LoadFileTest : public ::Test
    {
    public:
        LoadFileTest(TestFile testFile)
        : ::Test("LoadFileTest", __FILE__, __LINE__)
        , myTestFile(testFile)
        {
            
        }
        
        virtual bool getIsSlowTest() const override { return true; }
        
        std::unique_ptr<ContextHolder> createBitmapContext (size_t pixelsWide, size_t pixelsHigh)
        {
            auto holder = std::make_unique<ContextHolder>();
            size_t          bitmapByteCount;
            size_t          bitmapBytesPerRow;
            
            bitmapBytesPerRow   = (pixelsWide * 4);// 1
            bitmapByteCount     = (bitmapBytesPerRow * pixelsHigh);
            
            holder->colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
            holder->bitmapData = static_cast<CGColorSpaceRef>(calloc( bitmapByteCount, sizeof(CGContextRef)));
            if (!holder->bitmapData) {
                fprintf (stderr, "Memory not allocated!");
                return nullptr;
            }



//            context = CGBitmapContextCreate (bitmapData,
//                                             pixelsWide,
//                                             pixelsHigh,
//                                             8,
//                                             bitmapBytesPerRow,
//                                             colorSpace,
//                                             kCGImageAlphaPremultipliedLast);

            auto context = CGBitmapContextCreateWithData(holder->bitmapData,
                                                         pixelsWide,
                                                         pixelsHigh,
                                                         8,
                                                         bitmapBytesPerRow,
                                                         holder->colorSpace,
                                                         kCGImageAlphaPremultipliedLast,
                                                         ContextHolder::releaseCallback,
                                                         holder.get());

            if (!context) {
                free (holder->bitmapData);
                KOMP_THROW("Context not created!");
            }

            holder->context = context;
            return holder;
        }
        
        virtual void runTest(::TestResult& result) override
        {
            using namespace komp;
            auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
            bool isExceptionThrown = false;
            std::string exceptionMessage;
            
            try {
                auto score = fileMediator->loadFile(myTestFile.getFullPath());
//                const auto& page = score->getLayout()->getPages().front();
//                const auto& pageLayout = page->getLayout();
//                auto context = createBitmapContext(static_cast<size_t>(round(pageLayout.getPageWidth())),
//                                                   static_cast<size_t>(round(pageLayout.getPageHeight())));
//                score->draw();
                komp::FileSaveParams saveParams{*score.get()};
                fileMediator->saveFile(saveParams, "out-" + std::string(myTestFile.getFilename()));
            } catch (std::exception& e) {
                isExceptionThrown = true;
                exceptionMessage = e.what();
            }
            
            if (isExceptionThrown && !myTestFile.isLoadFailureExpected()) {
                std::stringstream ss;
                ss << myTestFile.getFilename() << " exception '";
                ss << exceptionMessage;
                ss << "'";
                ::Failure failure{ss.str(), getFileName(), getLineNumber(), getName()};
                failure.isException = true;
                result.addFailure(std::move(failure));
            }
            else if (!isExceptionThrown && myTestFile.isLoadFailureExpected()) {
                std::stringstream ss;
                ss << myTestFile.getFilename() << " was expected to throw an exception during load, but did not throw";
                ::Failure failure{ss.str(), getFileName(), getLineNumber(), getName()};
                failure.isException = true;
                result.addFailure(std::move(failure));
            }
        }
        
    private:
        TestFile myTestFile;
    };
    
    class LoadFileTestInstantiator
    {
    public:
        LoadFileTestInstantiator()
        {
            auto files = TestFileRepository::getNonExcludedFiles();
            for (const auto& file : files) {
                myTests.emplace_back(std::make_unique<LoadFileTest>(file));
            }
        }
    private:
        std::list<std::unique_ptr<LoadFileTest>> myTests;
    };

    
    LoadFileTestInstantiator instantiator;
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
