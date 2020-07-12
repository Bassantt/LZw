#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

using namespace std;

void encodedfunction()
{
    unordered_map<string, int> dic;
    int lengthdic = 0;

    ifstream inFile;
    ofstream basedictionary;
    inFile.open("enwik8"); //open the input file
    basedictionary.open("basedictionary.txt");
    stringstream strStream;
    strStream << inFile.rdbuf();  //read the file
    string str = strStream.str(); //str holds the content of the file

    cout << str.length() << "\n";

    for (int i = 0; i < str.length(); i++)
    {
        string stringValueOf = string(1, str[i]);
        auto result = dic.find(stringValueOf);
        if (result == dic.end())
        {
            dic.insert({stringValueOf, lengthdic});
            lengthdic++;
            basedictionary.put(str[i]);
        }
    }
    basedictionary.close();

    // //encode//
    int i = 0;
    int next = 1;

    string prev = string(1, str[0]);

    ofstream outencoded("encode", ios::binary);
    while (i < str.length())
    {
        string currentplusnext = prev;
        currentplusnext.push_back(str[next]);
        auto res = dic.find(currentplusnext);
        if (res != dic.end())
        { // exist

            prev = currentplusnext;
            i = next;
            next++;
        }
        else
        {
            auto res = dic.find(prev);
            outencoded.write(reinterpret_cast<char *>(&res->second), 3);
           // cout << res->second << endl;
            if (next == str.length())
            {
                break;
            }
            dic.insert({currentplusnext, lengthdic});
            lengthdic++;
            prev = string(1, str[next]);
            i = next;
            next++;
        }
    }

    outencoded.close();
}
void decodefunction()
{
    ifstream readfiletodecode;
    ifstream basedic;
    ofstream decodedfile;
    unordered_map<int, string> decdic;
    basedic.open("basedictionary.txt");
    int num = 0;
    basedic.seekg(0, ios::end);
    int sizebase = (int)basedic.tellg();
    basedic.seekg(0, ios::beg);
    while (basedic.tellg() < sizebase)
    {
        char car = basedic.get();
        decdic.insert({num, string(1, car)});
        cout <<string(1, car) ;
        num++;
    }

    basedic.close();
    decodedfile.open("decodedfile");
    readfiletodecode.open("encode", ios::in | ios::binary);
    readfiletodecode.seekg(0, ios::end);
    int size = (int)readfiletodecode.tellg();
    readfiletodecode.seekg(0, ios::beg);
    //cout << size << "   size" << endl;
    
    int reading;
    readfiletodecode.read(reinterpret_cast<char *>(&reading),3);
    string prevs = decdic[reading];
    string current = "";
    current =current+ prevs[0];
    decodedfile.put(prevs[0]);
    int oldreading = reading;

    while (readfiletodecode.tellg() < size-1)
    {
        readfiletodecode.read(reinterpret_cast<char *>(&reading),3);
        //cout << reading << endl;
        auto result = decdic.find(reading);

        if (result != decdic.end())
        {
           // cout << reading << " " << result->second << endl;
            prevs = decdic[reading];
        }
        else
        {
            prevs = decdic[oldreading];
            prevs =prevs + current;
        }
        for (int i = 0; i < prevs.length(); i++)
        {
            decodedfile.put(prevs[i]);
        }
        current = "";
        current += prevs[0];
        decdic.insert({num, decdic[oldreading] + current});
        num++;
        oldreading = reading;
    }
    readfiletodecode.close();
    decodedfile.close();
}
int main()
{ //encode
  encodedfunction();
    //decode
  //decodefunction();

    return 0;
}