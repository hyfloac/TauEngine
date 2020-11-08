#include "MemoryFileTest.hpp"
#include "UnitTest.hpp"
#include <MemoryFile.hpp>

TAU_TEST(MemoryFile, creationTest)
{
    {
        const auto file = MemoryFileLoader::Instance()->load(L"creationTest\\read.txt", FileProps::Read);
        TAU_EXPECT(!!file).printW(L"Unable to open read.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"creationTest\\writeNew.txt", FileProps::WriteNew);
        TAU_EXPECT(!!file).printW(L"Unable to open writeNew.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"creationTest\\writeOverwrite.txt", FileProps::WriteOverwrite);
        TAU_EXPECT(!!file).printW(L"Unable to open writeOverwrite.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"creationTest\\writeAppend.txt", FileProps::WriteAppend);
        TAU_EXPECT(!!file).printW(L"Unable to open writeAppend.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"creationTest\\readWrite.txt", FileProps::ReadWrite);
        TAU_EXPECT(!!file).printW(L"Unable to open readWrite.txt\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"creationTest\\read.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete read.txt\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"creationTest\\writeNew.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete writeNew.txt\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"creationTest\\writeOverwrite.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete writeOverwrite.txt\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"creationTest\\writeAppend.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete writeAppend.txt\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"creationTest\\readWrite.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete readWrite.txt\n");
    }
}

TAU_TEST(MemoryFile, reopenTest)
{
    {
        const auto file = MemoryFileLoader::Instance()->load(L"reopenTest\\read.txt", FileProps::Read);
        TAU_EXPECT(!!file).printW(L"Unable to open read.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"reopenTest\\writeNew.txt", FileProps::WriteNew);
        TAU_EXPECT(!!file).printW(L"Unable to open writeNew.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"reopenTest\\writeOverwrite.txt", FileProps::WriteOverwrite);
        TAU_EXPECT(!!file).printW(L"Unable to open writeOverwrite.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"reopenTest\\writeAppend.txt", FileProps::WriteAppend);
        TAU_EXPECT(!!file).printW(L"Unable to open writeAppend.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"reopenTest\\readWrite.txt", FileProps::ReadWrite);
        TAU_EXPECT(!!file).printW(L"Unable to open readWrite.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"reopenTest\\read.txt", FileProps::Read);
        TAU_EXPECT(!!file).printW(L"Unable to reopen read.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"reopenTest\\writeNew.txt", FileProps::WriteNew);
        TAU_EXPECT(!!file).printW(L"Unable to reopen writeNew.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"reopenTest\\writeOverwrite.txt", FileProps::WriteOverwrite);
        TAU_EXPECT(!!file).printW(L"Unable to reopen writeOverwrite.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"reopenTest\\writeAppend.txt", FileProps::WriteAppend);
        TAU_EXPECT(!!file).printW(L"Unable to reopen writeAppend.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"reopenTest\\readWrite.txt", FileProps::ReadWrite);
        TAU_EXPECT(!!file).printW(L"Unable to reopen readWrite.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"reopenTest\\readWrite.txt", FileProps::ReadWrite);
        TAU_EXPECT(!!file).printW(L"Unable to open readWrite.txt\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"reopenTest\\read.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete read.txt\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"reopenTest\\writeNew.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete writeNew.txt\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"reopenTest\\writeOverwrite.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete writeOverwrite.txt\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"reopenTest\\writeAppend.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete writeAppend.txt\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"reopenTest\\readWrite.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete readWrite.txt\n");
    }
}

TAU_TEST(MemoryFile, writeTest)
{
    const char* message = "Hello, World!";
    const uSys length = strlen(message);

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeTest\\read.txt", FileProps::Read);
        TAU_EXPECT(!!file).printW(L"Unable to reopen read.txt\n");

        const i64 writeCount = file->writeString(message);
        TAU_EXPECT_EQ(writeCount, -1).printW(L"Somehow able to write to a readonly file.\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeTest\\writeNew.txt", FileProps::WriteNew);
        TAU_EXPECT(!!file).printW(L"Unable to reopen writeNew.txt\n");

        const i64 writeCount = file->writeString(message);
        TAU_EXPECT_EQ(writeCount, length).printW(L"Unable to fully write to writeNew.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeTest\\writeOverwrite.txt", FileProps::WriteOverwrite);
        TAU_EXPECT(!!file).printW(L"Unable to reopen writeOverwrite.txt\n");

        const i64 writeCount = file->writeString(message);
        TAU_EXPECT_EQ(writeCount, length).printW(L"Unable to fully write to writeOverwrite.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeTest\\writeAppend.txt", FileProps::WriteAppend);
        TAU_EXPECT(!!file).printW(L"Unable to reopen writeAppend.txt\n");

        const i64 writeCount = file->writeString(message);
        TAU_EXPECT_EQ(writeCount, length).printW(L"Unable to fully write to writeAppend.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeTest\\readWrite.txt", FileProps::ReadWrite);
        TAU_EXPECT(!!file).printW(L"Unable to reopen readWrite.txt\n");

        const i64 writeCount = file->writeString(message);
        TAU_EXPECT_EQ(writeCount, length).printW(L"Unable to fully write to readWrite.txt\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"writeTest\\read.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete read.txt\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"writeTest\\writeNew.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete writeNew.txt\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"writeTest\\writeOverwrite.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete writeOverwrite.txt\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"writeTest\\writeAppend.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete writeAppend.txt\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"writeTest\\readWrite.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete readWrite.txt\n");
    }
}

TAU_TEST(MemoryFile, writeReadTest)
{
    const char* message = "Hello, World!";
    const uSys length = strlen(message);

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeReadTest\\read.txt", FileProps::Read);
        TAU_EXPECT(!!file).printW(L"Unable to reopen read.txt\n");

        const i64 writeCount = file->writeString(message);
        TAU_EXPECT_EQ(writeCount, -1).printW(L"Somehow able to write to a readonly file.\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeReadTest\\writeNew.txt", FileProps::WriteNew);
        TAU_EXPECT(!!file).printW(L"Unable to reopen writeNew.txt\n");

        const i64 writeCount = file->writeString(message);
        TAU_EXPECT_EQ(writeCount, length).printW(L"Unable to fully write to writeNew.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeReadTest\\writeOverwrite.txt", FileProps::WriteOverwrite);
        TAU_EXPECT(!!file).printW(L"Unable to reopen writeOverwrite.txt\n");

        const i64 writeCount = file->writeString(message);
        TAU_EXPECT_EQ(writeCount, length).printW(L"Unable to fully write to writeOverwrite.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeReadTest\\writeAppend.txt", FileProps::WriteAppend);
        TAU_EXPECT(!!file).printW(L"Unable to reopen writeAppend.txt\n");

        const i64 writeCount = file->writeString(message);
        TAU_EXPECT_EQ(writeCount, length).printW(L"Unable to fully write to writeAppend.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeReadTest\\readWrite.txt", FileProps::ReadWrite);
        TAU_EXPECT(!!file).printW(L"Unable to reopen readWrite.txt\n");

        const i64 writeCount = file->writeString(message);
        TAU_EXPECT_EQ(writeCount, length).printW(L"Unable to fully write to readWrite.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeReadTest\\read.txt", FileProps::Read);
        TAU_EXPECT(!!file).printW(L"Unable to reopen read.txt\n");

        char buf[13];
        const i64 readCount = file->readString(buf, 13);
        TAU_EXPECT_EQ(readCount, 0).printW(L"read.txt was larger than possible.\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeReadTest\\writeNew.txt", FileProps::WriteNew);
        TAU_EXPECT(!!file).printW(L"Unable to reopen writeNew.txt\n");
        
        char buf[13];
        const i64 readCount = file->readString(buf, 13);
        TAU_EXPECT_EQ(readCount, -1).printW(L"Somehow able to read write-only file writeNew.txt.\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeReadTest\\writeOverwrite.txt", FileProps::WriteOverwrite);
        TAU_EXPECT(!!file).printW(L"Unable to reopen writeOverwrite.txt\n");
        
        char buf[13];
        const i64 readCount = file->readString(buf, 13);
        TAU_EXPECT_EQ(readCount, -1).printW(L"Somehow able to read write-only file writeOverwrite.txt.\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeReadTest\\writeAppend.txt", FileProps::WriteAppend);
        TAU_EXPECT(!!file).printW(L"Unable to reopen writeAppend.txt\n");
        
        char buf[13];
        const i64 readCount = file->readString(buf, 13);
        TAU_EXPECT_EQ(readCount, -1).printW(L"Somehow able to read write-only file writeAppend.txt.\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeReadTest\\readWrite.txt", FileProps::ReadWrite);
        TAU_EXPECT(!!file).printW(L"Unable to reopen readWrite.txt\n");

        char buf[13];
        const i64 readCount = file->readString(buf, 13);
        TAU_EXPECT_EQ(readCount, length).printW(L"readWrite.txt was an impossible size.\n");

        TAU_EXPECT_EQ(const_cast<const char*>(buf), message).printW(L"readWrite.txt got corrupted.\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"writeReadTest\\read.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete read.txt\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"writeReadTest\\writeNew.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete writeNew.txt\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"writeReadTest\\writeOverwrite.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete writeOverwrite.txt\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"writeReadTest\\writeAppend.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete writeAppend.txt\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"writeReadTest\\readWrite.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete readWrite.txt\n");
    }
}

TAU_TEST(MemoryFile, writeNewTest)
{
    const char* message = "Hello, World!";
    const uSys length = strlen(message);

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeNewTest\\writeNew.txt", FileProps::WriteNew);
        TAU_EXPECT(!!file).printW(L"Unable to open writeNew.txt\n");

        const i64 writeCount = file->writeString(message);
        TAU_EXPECT_EQ(writeCount, length).printW(L"Unable to fully write to writeNew.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeNewTest\\writeNew.txt", FileProps::WriteNew);
        TAU_EXPECT(!!file).printW(L"Unable to reopen writeNew.txt\n");

        const i64 fileSize = file->size();
        TAU_EXPECT_EQ(fileSize, 0).printW(L"writeNew.txt was larger than should be possible.\n");

        const i64 writeCount = file->writeString(message);
        TAU_EXPECT_EQ(writeCount, length).printW(L"Unable to fully write to writeNew.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeNewTest\\writeNew.txt", FileProps::Read);
        TAU_EXPECT(!!file).printW(L"Unable to reopen writeNew.txt\n");
        
        char buf[13];
        const i64 readCount = file->readString(buf, length);
        TAU_EXPECT_EQ(readCount, length).printW(L"Unable to read message from writeNew.txt.\n");

        TAU_EXPECT_EQ(const_cast<const char*>(buf), message).printW(L"writeNew.txt got corrupted.\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"writeNewTest\\writeNew.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete writeNew.txt\n");
    }
}

TAU_TEST(MemoryFile, writeOverwriteTest)
{
    const char* message = "Hello, World!";
    const uSys length = strlen(message);

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeOverwriteTest\\writeOverwrite.txt", FileProps::WriteOverwrite);
        TAU_EXPECT(!!file).printW(L"Unable to open writeOverwrite.txt\n");

        const i64 writeCount = file->writeString(message);
        TAU_EXPECT_EQ(writeCount, length).printW(L"Unable to fully write to writeOverwrite.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeOverwriteTest\\writeOverwrite.txt", FileProps::WriteOverwrite);
        TAU_EXPECT(!!file).printW(L"Unable to reopen writeOverwrite.txt\n");

        const i64 fileSize = file->size();
        TAU_EXPECT_EQ(fileSize, length).printW(L"writeNew.txt was an impossible size.\n");

        const i64 writeCount = file->writeString(message);
        TAU_EXPECT_EQ(writeCount, length).printW(L"Unable to fully write to writeOverwrite.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeOverwriteTest\\writeOverwrite.txt", FileProps::Read);
        TAU_EXPECT(!!file).printW(L"Unable to ropen writeOverwrite.txt\n");
        
        char buf[13];
        const i64 readCount = file->readString(buf, 13);
        TAU_EXPECT_EQ(readCount, length).printW(L"Unable to read message from writeOverwrite.txt.\n");

        TAU_EXPECT_EQ(const_cast<const char*>(buf), message).printW(L"writeOverwrite.txt got corrupted.\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"writeOverwriteTest\\writeOverwrite.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete writeOverwrite.txt\n");
    }
}

TAU_TEST(MemoryFile, writeAppendTest)
{
    const char* message = "Hello, World!";
    const uSys length = strlen(message);
    const char* doubleMessage = "Hello, World!Hello, World!";
    const uSys doubleLength = length * 2;

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeAppendTest\\writeAppend.txt", FileProps::WriteAppend);
        TAU_EXPECT(!!file).printW(L"Unable to open writeAppend.txt\n");

        const i64 writeCount = file->writeString(message);
        TAU_EXPECT_EQ(writeCount, length).printW(L"Unable to fully write to writeAppend.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeAppendTest\\writeAppend.txt", FileProps::WriteAppend);
        TAU_EXPECT(!!file).printW(L"Unable to reopen writeAppend.txt\n");

        const i64 fileSize = file->size();
        TAU_EXPECT_EQ(fileSize, length).printW(L"writeNew.txt was an impossible size.\n");

        const i64 writeCount = file->writeString(message);
        TAU_EXPECT_EQ(writeCount, length).printW(L"Unable to fully write to writeAppend.txt\n");
    }

    {
        const auto file = MemoryFileLoader::Instance()->load(L"writeAppendTest\\writeAppend.txt", FileProps::Read);
        TAU_EXPECT(!!file).printW(L"Unable to reopen writeAppend.txt\n");
        
        char buf[26];
        const i64 readCount = file->readString(buf, 26);
        TAU_EXPECT_EQ(readCount, doubleLength).printW(L"Unable to read message from writeAppend.txt.\n");

        TAU_EXPECT_EQ(const_cast<const char*>(buf), doubleMessage).printW(L"writeAppend.txt got corrupted.\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"writeAppendTest\\writeAppend.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete writeAppend.txt\n");
    }
}

TAU_TEST(MemoryFile, deleteTest)
{
    {
        const auto file = MemoryFileLoader::Instance()->load(L"deleteTest\\test.txt", FileProps::WriteNew);
        TAU_EXPECT(!!file).printW(L"Unable to open test.txt\n");

        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"deleteTest\\test.txt");
        TAU_EXPECT(!deleted).printW(L"Somehow able to delete test.txt while it was still open.\n");
    }

    {
        const auto deleted = MemoryFileLoader::Instance()->deleteFile(L"deleteTest\\test.txt");
        TAU_EXPECT(deleted).printW(L"Unable to delete test.txt\n");
    }
}

void MemoryFileTest::runTests()
{
    RUN_ALL_TESTS();
}
