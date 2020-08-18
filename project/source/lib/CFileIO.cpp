//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	CFileIO.cpp
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <CFileIO.h>

using namespace std;


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   GetChar
//
// Parameter: char* Filename - Name der Datei
//
// Return:    Eingelesenes Zeichen
//
//---------------------------------------------------------------------------

int CFileIO::GetChar()
{
    char ch;

    mFs->get(ch);
    return ch;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   IsEof
//
//
//---------------------------------------------------------------------------

bool CFileIO::IsEof()
{
    if (mFs == NULL)
    {
        return true;
    }
    return mFs->eof();
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   Clear
//
//
//
//
//---------------------------------------------------------------------------

void CFileIO::Clear()
{
    if (mFs != NULL)
    {
        mFs->clear();
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   SkipBytes
//
// Parameter: int nBytes - Anzahl an Bytes ueberspringen (Eingabedatei)
//
//
//---------------------------------------------------------------------------

void CFileIO::SkipBytes(int nBytes)
{
    mFs->seekg(nBytes, ios_base::cur);
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   GetFileSize
//
// Parameter: char* Filename - Name der Datei
//
// Return:    int - Groesse der Datei in Bytes
//
//---------------------------------------------------------------------------

int CFileIO::GetFileSize(const char* Filename)
{
    long FileSize;
    ifstream fin(Filename, ios::in | ios::binary);

    fin.seekg(0, ios::end);
    FileSize = fin.tellg();
    fin.close();
    return FileSize;
}


//--------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   FileExists
//
// Parameter: char* Filename - Name der Datei
//
// Return:    true  - Datei wurde geoeffnet
//            false - Datei konnte nicht geöffnet werden
//
// ueberpruefen, ob Datei existiert
//
//---------------------------------------------------------------------------

/*
 * bool CFileIO::FileExists(const char* Filename)
 * {
 * ifstream fin(Filename, ios::in | ios::binary);
 * bool s = fin.good();
 *
 * if (s)
 * {
 *  fin.close();
 * }
 * return s;
 * }
 */
bool CFileIO::FileExists(const char* Filename)
{
    struct stat buffer;

    return stat(Filename, &buffer) == 0;

    //return access(Filename, F_OK) != -1;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		FileAttribut
//
// return: 0 = Datei nicht gefunden
//         1 = "normale" Datei
//         2 = Ordner
//
//---------------------------------------------------------------------------

/*
 * int CFileIO::FileAttribut(const char* Filename)
 * {
 *  int r = EFA_FILE;
 *
 #ifdef __WIN32__
 *
 *  DWORD fa = GetFileAttributes(Filename);
 *  if (fa == 0xFFFFFFFF)
 *  {
 *      r = EFA_NOT_FOUND;
 *  }
 *  else
 *  if (fa & FILE_ATTRIBUTE_DIRECTORY)
 *  {
 *      r = EFA_FOLDER;
 *  }
 #endif
 *
 *  return r;
 *
 * }
 */

//--------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   OpenFileRead
//
// Parameter: char* Filename - Name der Datei
//
// Return:    true  - Datei wurde geoeffnet
//            false - Datei konnte nicht geoeffnet werden
//
// Öffnen einer Datei zum Lesen
//
//---------------------------------------------------------------------------

bool CFileIO::OpenFileRead(const char* Filename, OPENMODE OpenMode)
{
    if (mFs != NULL)
    {
        delete mFs;
    }
    mFs = new fstream(Filename, ios::in | OpenMode);
    mOpenRead = mFs->good();
    return mOpenRead;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   OpenFileWrite
//
// Parameter: char* Filename - Name der Datei
//
// Return:    true  - Datei wurde geöffnet
//            false - Datei konnte nicht geöffnet werden
//
// Öffnen einer Datei zum Lesen
//
//---------------------------------------------------------------------------

bool CFileIO::OpenFileWrite(const char* Filename, OPENMODE OpenMode)
{
    if (mFs != NULL)
    {
        delete mFs;
    }
    mFs = new fstream(Filename, ios::out | OpenMode);
    mOpenWrite = mFs->good();
    return mOpenWrite;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   OpenFileAppend
//
// Parameter: char* Filename - Name der Datei
//
// Return:    true  - Datei wurde geöffnet
//            false - Datei konnte nicht geöffnet werden
//
// Öffnen einer Datei zum Lesen
//
//---------------------------------------------------------------------------

bool CFileIO::OpenFileAppend(const char* Filename, OPENMODE OpenMode)
{
    if (mFs != NULL)
    {
        delete mFs;
    }
    mFs = new fstream(Filename, ios::app | ios::out| OpenMode);
    mOpenWrite = mFs->good();
    return mOpenWrite;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   OpenFileChange
//
// Parameter: char* filename - Name der Datei
//            int pos        - Schreibposition
//
// Return:    true  - Datei wurde geöffnet
//            false - Datei konnte nicht geöffnet werden
//
// Oeffnen einer Datei zum Aendern
//
//---------------------------------------------------------------------------

bool CFileIO::OpenFileChange(const char* Filename, int Position)
{
    bool suc = OpenFileWrite(Filename, ios::out | ios::binary | ios::in);

    if (suc)
    {
        SetReadPos(Position);
        SetWritePos(Position);
    }
    return suc;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadFile
//
// Parameter: char* Filename - Name der Datei
//
//            int* nBytes    - Anzahl der Bytes die gelesen wurden (Größe
//                             der Datei).
//
// Return:    void* - Zeiger auf Puffer, in den die Daten gelesen wurden,
//                    oder NULL (Fehlerfall)
//
//
//
// Lesen des Inhalts einer Datei in einen Puffer. Der Speicher für den
// Puffer wird reserviert. Zurückgegeben werden ein Zeiger auf den
// Puffer sowie die Anzahl der eingelesenen Bytes
//
//---------------------------------------------------------------------------

void* CFileIO::ReadFile(const char* Filename, int* nBytes)
{
    CFileIO fio;
    char* buffer = NULL;
    int filesize = CFileIO::GetFileSize(Filename);

    if (filesize > 0)
    {
        if (fio.OpenFileRead(Filename, ios::binary))
        {
            buffer = new char[filesize+1];
            if (buffer != NULL)
            {
                fio.ReadBytes(buffer, filesize);
                buffer[filesize] = 0;
            }
        }
        fio.CloseFile();
    }

    if (nBytes != nullptr)
    {
        *nBytes = filesize;
    }

    return buffer;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   WriteFile
//
// Parameter: char* Filename - Name der Datei
//
//            int   nBytes    - Anzahl der Bytes die gelesen wurden (Größe
//                             der Datei).
//
//            const void* - Zeiger auf Puffer, dessen Daten geschrieben
//                          werden
//
//
//---------------------------------------------------------------------------

bool CFileIO::WriteFile(const char* Filename, const void* buffer, int nBytes)
{
    bool suc = false;

    if (nBytes > 0)
    {
        if (OpenFileWrite(Filename, ios::binary))
        {
            WriteBytes(buffer, nBytes);
            suc = true;
            CloseFile();
        }
    }
    return suc;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   WriteBytes
//
//
//
// Einlesen einer Anzahl von Bytes aus einer geöffneten Datei
//
//---------------------------------------------------------------------------

bool CFileIO::WriteBytes(const void* buffer, int nBytes)
{
    if (mOpenWrite)
    {
        mFs->write((const char*)buffer, nBytes);
    }
    return mOpenWrite;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   WriteString
//
//
//---------------------------------------------------------------------------

bool CFileIO::WriteString(const char* str)
{
    return WriteBytes(str, strlen(str));
}


#if 0
//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReplaceAllStrings
//
//
//---------------------------------------------------------------------------

class CStrpos : public CListElem<CStrpos>
{
public:

    char* mPos;
};

class CStrposList : public CList<CStrpos>
{
};


static void CopyStringBytes(char* dst, char* src, int num)
{
    int i;

    for (i = 0; (i < num) && (*src != 0); i++)
    {
        *(dst++) = *(src++);
    }
    *dst = 0;
}


char* CFileIO::ReplaceAllStrings(char* SrcString, char* SearchString, char* SubString, int* Count)
{
    CStrposList slist;
    int cnt = 0;
    char* pos;
    char* StartPos = SrcString;
    int LenSearch = strlen(SearchString);
    int LenSub = strlen(SubString);
    int LenNew;
    char* NewString = NULL;

    do
    {
        pos = strstr(StartPos, SearchString);
        if (pos != NULL)
        {
            CStrpos* se = slist.NewListElem();
            se->mPos = pos;
            StartPos = pos + LenSearch;
            cnt++;
        }
    }while (pos != NULL);

    *Count = cnt;

    if (cnt > 0)
    {
        LenNew = strlen(SrcString) + (cnt * (LenSub - LenSearch));

        NewString = new char[LenNew+1];
        char* NewStringPos = NewString;

        StartPos = SrcString;
        for (CStrpos* se = slist.GetFirst();
            se != NULL;
            se = slist.GetNext())
        {
            int len = se->mPos - StartPos;
            CopyStringBytes(NewStringPos, StartPos, len);
            NewStringPos += len;
            CopyStringBytes(NewStringPos, SubString, LenSub);
            NewStringPos += LenSub;

            StartPos = se->mPos + LenSearch;
        }
        CopyStringBytes(NewStringPos, StartPos, strlen(StartPos));
        slist.ClearList();
    }
    else
    {
        NewString = new char[strlen(SrcString) + 1];
        strcpy(NewString, SrcString);
    }

    return NewString;
}


#endif

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadBytes
//
// Parameter: void* buffer - Zeiger auf Puffer, in den die Datei eingelesen
//                           werden
//
//            int nBytes   - Anzahl der Bytes, die eingelesen werden sollen
//
// Return:    true  - Daten wurden eingelesen
//            false - Daten konnten nicht eingelesen werden
//
//
//
// Einlesen einer Anzahl von Bytes aus einer geöffneten Datei
//
//---------------------------------------------------------------------------

bool CFileIO::ReadBytes(void* buffer, int nBytes)
{
    if (mOpenRead)
    {
        mFs->read((char*)buffer, nBytes);
    }
    return mOpenRead;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadLine
//
// Einlesen einer Text-Zeile aus einer Datei
// Die Datei muss im Binär-Modus geöffnet sein
//
//---------------------------------------------------------------------------

int CFileIO::ReadLine(vector<char>* Line)
{
    char ch;
    bool EndLine = false;

    Line->clear();
    do
    {
        mFs->get(ch);
        if (!mFs->eof())
        {
            if (ch != 0x0D)
            {
                if (ch != 0x0A)
                {
                    Line->push_back(ch);
                }
                else
                {
                    EndLine = true;
                }
            }
        }
        else
        {
            EndLine = true;
        }
    }while (!EndLine);
    return true;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadLine
//
// Einlesen einer Text-Zeile aus einer Datei
// Die Datei muss im Binär-Modus geoeffnet sein
//
//---------------------------------------------------------------------------

int CFileIO::ReadLine(string* Line)
{
    char ch;
    bool EndLine = false;
    bool IsEof = false;

    Line->clear();
    do
    {
        mFs->get(ch);
        if (!mFs->eof())
        {
            if (ch != 0x0D)
            {
                if (ch != 0x0A)
                {
                    Line->push_back(ch);
                }
                else
                {
                    EndLine = true;
                }
            }
        }
        else
        {
            EndLine = true;
            IsEof = true;
        }
    }while (!EndLine);
    return !IsEof;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadSplitLine
//
// Parameter:
//
// Return:
//
//
// Lesen von Bytes aus einer Datei
//
//---------------------------------------------------------------------------

int CFileIO::ReadSplitLine(stringvector* StringVector, char Div)
{
    string LineBuffer;
    int Eof = ReadLine(&LineBuffer);

    StringVector->clear();
    NStringTool::Split(LineBuffer, StringVector, Div);

    return Eof;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadBytes
//
// Parameter:
//
// Return:
//
//
// Lesen von Bytes aus einer Datei
//
//---------------------------------------------------------------------------

void* CFileIO::ReadBytes(int nBytes)
{
    char* buffer = NULL;

    if (mOpenRead)
    {
        buffer = new char[nBytes];
        mFs->read(buffer, nBytes);
    }
    return buffer;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   CloseFile
//
// Parameter:
//
// Return:
//
//
// Schliessen einer Datei
//
//---------------------------------------------------------------------------

void CFileIO::CloseFile()
{
    if (mOpenWrite || mOpenRead)
    {
        mFs->close();
        mOpenRead = false;
        mOpenWrite = false;
        delete mFs;
        mFs = NULL;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   SetReadPos
//
//
//
//
//---------------------------------------------------------------------------

bool CFileIO::SetReadPos(int pos)
{
    mFs->seekg(pos, ios::beg);
    return (int)mFs->tellg() == pos;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   SetReadPos
//
//
//
//
//---------------------------------------------------------------------------

bool CFileIO::SetReadPos2(int pos, SEEKDIR where)
{
    mFs->seekg(pos, where);
    return (int)mFs->tellg() == pos;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   SetReadPos
//
//
//
//
//---------------------------------------------------------------------------

void CFileIO::SetWritePosEnd()
{
    mFs->seekp(0, ios_base::end);
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   SetReadPos
//
//
//
//
//---------------------------------------------------------------------------

void CFileIO::SetWritePos(unsigned int pos)
{
    mFs->seekp(pos, ios_base::beg);
    //mFs->seekg(pos, ios_base::beg);
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   GetReadPos
//
//
//
//
//---------------------------------------------------------------------------

int CFileIO::GetReadPos()
{
    return mFs->tellg();
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   GetWritePos
//
//
//
//
//---------------------------------------------------------------------------

int CFileIO::GetWritePos()
{
    return mFs->tellp();
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		GuessFileFormat
//
// Return:
//
//  EFILETYPE_UNKNOWN,
//  EFILETYPE_JPG,
//  EFILETYPE_TIF,
//  EFILETYPE_PNG,
//  EFILETYPE_TGA,
//  EFILETYPE_RAW
//
//---------------------------------------------------------------------------

EFileTypes CFileIO::GuessFileFormat(const char* Filename)
{
    const char* AppList[] =
    {
        ".IDAT",
        ".IDX",
        ".DAT",
        ".JPG",
        ".TIF",
        ".PNG",
        ".TGA",
        ".RAW",
        ".GIF",
        NULL
    };

    int FileType;
    char FilenameCopy[256];

    strcpy(FilenameCopy, Filename);

    if (strlen(FilenameCopy) > 4)
    {
        for (int i = 0; FilenameCopy[i] != 0; i++)
        {
            FilenameCopy[i] = toupper(FilenameCopy[i]);
        }

        for (int i = 0; AppList[i] != NULL; i++)
        {
            char* FilenameCopyPtr = FilenameCopy + strlen(FilenameCopy) - strlen(AppList[i]);
            if (strcmp(FilenameCopyPtr, AppList[i]) == 0)
            {
                return (EFileTypes)(i+1);
            }
        }
    }
    return EFILETYPE_UNKNOWN;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		ReplaceApp
//
//---------------------------------------------------------------------------

char* CFileIO::ReplaceApp(const char* Filename, const char* neuApp)
{
    char NameBuffer[512];
    char* NeuFilename;

    strcpy(NameBuffer, Filename);
    char* Punkt = strrchr(NameBuffer, '.');

    if (Punkt != NULL)
    {
        *Punkt = 0;
    }

    strcat(NameBuffer, neuApp);

    NeuFilename = new char[strlen(NameBuffer) + 1];

    strcpy(NeuFilename, NameBuffer);

    return NeuFilename;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		Strcmpx
//
//---------------------------------------------------------------------------

bool CFileIO::Strcmpx(const char* s1, const char* s2)
{
    if ((s1 == NULL) || (s2 == NULL))
    {
        return false;
    }

    char* bstr1 = new char[strlen(s1) + 1];
    char* bstr2 = new char[strlen(s2) + 1];

    strcpy(bstr1, s1);
    strcpy(bstr2, s2);

    for (char* c1 = bstr1; *c1 != 0; c1++)
    {
        *c1 = toupper(*c1);
    }
    for (char* c1 = bstr2; *c1 != 0; c1++)
    {
        *c1 = toupper(*c1);
    }

    int s = strcmp(bstr1, bstr2);

    delete bstr2;
    delete bstr1;

    return s == 0;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		ResolveWildCard
//
//---------------------------------------------------------------------------

bool CFileIO::ResolveWildCard(const char* Dir, const char* WildCard, stringvector* FileList)
{
    struct stat FileInfo;
    struct dirent* CurrentFile;
    DIR* Directory;
    char** DirList = NULL;
    bool r = false;

    Directory = opendir(Dir);

    cout << "CFileIO::ResolveWildCard Dir=" << Dir << " Wildcard=" << WildCard << endl;

    if (WildCard[0] == '*')
    {
        WildCard++;
    }
    FileList->clear();

    if (Directory != NULL)
    {
        r = true;
        Directory = opendir(Dir);

        int i = 0;
        while ((CurrentFile = readdir(Directory)) != NULL)
        {
            char* EndString = CurrentFile->d_name + (strlen(CurrentFile->d_name) - strlen(WildCard));

            if (Strcmpx(EndString, WildCard))
            {
                string Filename = CurrentFile->d_name;
                FileList->push_back(Filename);
            }

            //cout << "File:" << CurrentFile->d_name << endl;
        }

        closedir(Directory);
    }
    return r;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		StripExtension
//
// Liefert den Dateinamen ohne Datei-Anhang zurueck
//
//---------------------------------------------------------------------------

string CFileIO::StripExtension(const char* FullPathName)
{
    string OutString;
    char* TmpName = NewString(FullPathName);
    char* slash = strrchr(TmpName, '.');

    if (slash != NULL)
    {
        *slash = 0;
        OutString = TmpName;
    }
    else
    {
        OutString = FullPathName;
    }
    return OutString;
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CFileIO
// Methode: Fill0
//
//   Integer-Zahl in String umwandeln und dabei mit Nullen
//   auffüllen.
//
//   int i - Zahl
//   int n - Anzahl der Stellen
//
//---------------------------------------------------------------------------

char* CFileIO::Fill0(int i, int n)
{
    static char strbuffer[16];

    return Fill0(strbuffer, i, n);
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CFileIO
// Methode: Fill0
//
//   Integer-Zahl in String umwandeln und dabei mit Nullen
//   auffüllen.
//
//   int i - Zahl
//   int n - Anzahl der Stellen
//
//---------------------------------------------------------------------------

string CFileIO::Fill0str(int i, int n)
{
    string fstr = Fill0(i, n);

    return fstr;
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CFileIO
// Methode: Fill0
//
//   Integer-Zahl in String umwandeln und dabei mit Nullen
//   auffüllen.
//
//   int i - Zahl
//   int n - Anzahl der Stellen
//
//---------------------------------------------------------------------------

char* CFileIO::Fill0(char* strbuffer, int i, int n)
{
    sprintf(strbuffer, "%d", i);

    char len = strlen(strbuffer);

    if (len < n)
    {
        char strbuffer2[16];

        strcpy(strbuffer2, strbuffer);
        strcpy(strbuffer + (n - len), strbuffer2);

        for (int j = 0; j < n - len; j++)
        {
            strbuffer[j] = '0';
        }
    }
    return strbuffer;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   CreateDir
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

/*
 * bool CFileIO::CreateDir(const char* NewFolder)
 * {
 #if defined __WIN32__
 *
 *  SECURITY_ATTRIBUTES sa;
 *  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
 *  sa.lpSecurityDescriptor = NULL;
 *  sa.bInheritHandle = TRUE;
 *  return CreateDirectory(NewFolder, &sa);
 *
 #elif defined __unix__
 *
 *  mkdir(NewFolder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
 *
 *  return 1;
 *
 #else
 *  FEHLER CFileIO::CreateDir
 #endif
 *  return 0;
 * }
 */
//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   NewString
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

char* CFileIO::NewString(const char* OldStr)
{
    char* NewStr = NULL;

    if (OldStr != NULL)
    {
        NewStr = new char[strlen(OldStr) + 1];
        strcpy(NewStr, OldStr);
    }
    return NewStr;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		ExtractFilename
//
//---------------------------------------------------------------------------

char* CFileIO::ExtractFilename(const char* FullPathName)
{
    const char* slash = strrchr(FullPathName, '/');

    if (slash == NULL)
    {
        slash = strrchr(FullPathName, '\\');

        if (slash == NULL)
        {
            return NewString(FullPathName);
        }
    }
    return NewString(slash + 1);
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   WriteFromFile
//
//
//---------------------------------------------------------------------------

int CFileIO::WriteFromFile(const char* Filename)
{
    int FileSize = 0;
    const char* FileData = (const char*)ReadFile(Filename, &FileSize);

    if (FileData != NULL)
    {
        WriteBytes(FileData, FileSize);
        delete[] FileData;
    }
    return FileSize;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		ExtractPath
//
//---------------------------------------------------------------------------

const char* CFileIO::ExtractPath(const char* FullPathName)
{
    char* PathName = new char[strlen(FullPathName) + 1];

    strcpy(PathName, FullPathName);

    char* slash = strrchr(PathName, '\\');

    // cout << "Slash=" << slash << endl;

    if (slash != NULL)
    {
        slash[1] = 0;
    }
    else
    {
        slash = strrchr(PathName, '/');
        if (slash != NULL)
        {
            slash[1] = 0;
        }
    }
    return PathName;
}
