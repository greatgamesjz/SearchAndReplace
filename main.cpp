#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <string.h>
#include <dirent.h>
using namespace std;
void replaceCode(string path,string sourceFindString, string sourceChangeString, bool askMe)
{
    string firstPart, finalString;
    ifstream destination;
    fstream plik;
    string tmp;
    int found;
    int endOfSource;
    destination.open(path);
    if(destination.good())
    {
        do
        {
            string destinationString((std::istreambuf_iterator<char>(destination)),std::istreambuf_iterator<char>());
            destination.close();
            found = destinationString.find(sourceFindString);
            endOfSource = found+sourceFindString.length();
            if(found != string::npos)
            {
                firstPart=destinationString.substr(0,found);
                firstPart+=sourceChangeString;
                firstPart+= destinationString.substr(endOfSource,(destinationString.length()-endOfSource));
                destinationString.erase(found,endOfSource);
                finalString= firstPart;
                plik.open(path, ios::out | ios::trunc);
                cout<<"Edytuje: "<<path<<"[t/n]"<<endl;
                if(askMe)
                {
                    do
                    {
                        cin>>tmp;
                        if(tmp=="t")
                            plik << finalString;
                    }while(tmp!="t" && tmp!="n");
 
                }
                plik.close();
                destination.open(path);
            }
        }while(found != string::npos);
        destination.close();
    }
 
}
void searchDirs(string path, string sourceFindString, string sourceChangeString,bool firstDir,bool askMe)
{
    DIR *dir;
    string path2;
    struct dirent *ent;
 
    if (((dir = opendir (path.c_str())) != NULL))
    {
 
        while ((ent = readdir (dir)) != NULL)
        {
            if(strcmp(ent->d_name, "..") && strcmp(ent->d_name, "."))
            {
 
                path2=path;
                path2+="/";
                path2+=ent->d_name;
                if(ent->d_type==DT_DIR)
                {
 
                    searchDirs(path2,sourceFindString,sourceChangeString,1,askMe);
 
                }
                else if(firstDir && ent->d_type==DT_REG)
                {
                    replaceCode(path2,sourceFindString,sourceChangeString, askMe);
                }
            }
        }
    }
    closedir (dir);
 
}
int main(int argc, char** argv)
{
    string codeToFind, codeToChange;
    string finalString;
    ifstream sourceFind;
    ifstream sourceChange;
    fstream plik;
    string tmp;
    bool askMe=false;
    string path;
    size_t found;
    if(argc>1 && argc<4)
        return 0;
    if(argc >= 4)
    {
        codeToFind=argv[1];
        codeToChange=argv[2];
        path=argv[3];
        sourceFind.open(codeToFind);
        sourceChange.open(codeToChange);
        if(argc>4 && argv[4]=="ask")
            askMe=true;
    }
    else
    {
        do
        {
            cout<<"Podaj nazwe pliku z kodem ktory mam zamienic: ";
            cin>>codeToFind;
            sourceFind.open(codeToFind);
            if(sourceFind.good())
            {
                cout<<"Plik zostal pomyslnie zaladowany"<<endl;
                break;
            }
            else
            {
                cout<<"Nie znaleziono podanego pliku :("<<endl;
            }
        }while(true);
        do
        {
            cout<<"Podaj nazwe pliku z kodem na ktory mam zamienic: ";
            cin>>codeToChange;
            sourceChange.open(codeToChange);
            if(sourceChange.good())
            {
                cout<<"Plik zostal pomyslnie zaladowany"<<endl;
                break;
            }
            else
            {
                cout<<"Nie znaleziono podanego pliku :("<<endl;
            }
        }while(true);
 
 
        cout<<"Podaj sciezke do folderu w ktorym mam uruchomic skrypt"<<endl;
        cout<<"Nie zjeb bo może być niewesolo"<<endl;
        cout<<"Przykładowa sciezka: '/home/witkowskid/Dokumenty/porno"<<endl;
        cout<<"SCIEZKA: ";
        cin>>path;
        do
        {
            cout<<"Czy mam pytac przed edycja?[t/n]: "<<endl;
            cin>>tmp;
            if(tmp=="t")
                askMe=true;
        }while(tmp!="t" && tmp!="n");
    }
    string sourceFindString((std::istreambuf_iterator<char>(sourceFind)),std::istreambuf_iterator<char>());
    sourceFind.close();
    string sourceChangeString((std::istreambuf_iterator<char>(sourceChange)),std::istreambuf_iterator<char>());
    sourceChange.close();
 
 
    cout<<"------------SHOWTIME------------"<<endl;
 
 
    searchDirs(path,sourceFindString,sourceChangeString,0,askMe);
    return 0;
}
