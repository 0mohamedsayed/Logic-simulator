//============================================================================
// Name        : SI_vision.cpp
// Author      : sayed
// Version     : 01
// Copyright   : open source
// Description : Logic Simulator
//============================================================================

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include<string.h>
using namespace std;


class Node
{
private:
    bool value;
    string name;

public:
    Node(const string& Name ):value(0),name(Name){}


    string getName() const {
        return name;
    }

    void setName(string name) {
        this->name = name;
    }

    bool getValue() const {
        return value;
    }

    void setValue(bool value) {
        this->value = value;
    }

bool And(Node& obj)
{

    return this->value & obj.value;
}

bool OR(Node& obj)
{

    return this->value | obj.value;
}

bool XOR(Node& obj)
{

    return this->value ^ obj.value;
}

friend ostream& operator<<(ostream& out, const Node& nodeobj);
};



ostream& operator<<(ostream& out, const Node& nodeobj) {

out<<"Node name : "<<nodeobj.name<<endl<<"Node value : "<<nodeobj.value;

return out;
}



class Gate
{

private:
    Node& inputone;
    Node& inputtwo;
    Node& output;
    string TYPE;

public:

    Gate(const string& type , Node& inone ,Node& intwo , Node& out):inputone(inone),inputtwo(intwo),output(out),TYPE(type)
    {

    }

    bool simulateGate()
    {
    bool value;
    if(TYPE == "AND")
    {
        value=inputone.And(inputtwo);
        output.setValue(value);
    }
    else if(TYPE == "OR")
    {
        value=inputone.OR(inputtwo);
        output.setValue(value);
    }
    else if(TYPE == "NAND")
    {
        value=inputone.And(inputtwo);
        output.setValue(!value);
    }
    else if(TYPE == "NOR")
    {
        value=inputone.OR(inputtwo);
        output.setValue(!value);
    }
    else if(TYPE == "XOR")
    {
        value=inputone.XOR(inputtwo);
        output.setValue(value);
    }
    else if(TYPE == "XNOR")
    {
        value=inputone.XOR(inputtwo);
        output.setValue(!value);
    }
    else if(TYPE == "NOT")
    {
       value=!inputone.getValue();
       inputtwo.setValue(value);

    }

    return output.getValue();
    }

    Node getInputone() const {
        return inputone;
    }

    void setInputone(Node inputone) {
        this->inputone = inputone;
    }

    Node getInputtwo() const {
        return inputtwo;
    }

    void setInputtwo(Node inputtwo) {
        this->inputtwo = inputtwo;
    }

    Node getOutput() const {
        return output;
    }

    void setOutput(Node output) {
        this->output = output;
    }
};



class Simulator
{

private:
    vector<Node*> nodearr;
    vector<Gate*> gatearr;
    Simulator(){}
    Simulator(const Simulator& other) = delete;

public:
    void postGate(Gate*& gate_elm)
    {
        gatearr.push_back(gate_elm);

    }

    void postNode(Node*& node_elm)
    {
        nodearr.push_back(node_elm);

    }

    Node* FindNode(string name)
    {
      int j=-1;
      Node* ptr =nullptr;
      for(int i=0  ; i < (static_cast<int> (nodearr.size())) ; i++)
      {

         if(nodearr.at(i)->getName() == name)
         {
             j=i;
         }
      }

      if(j!=-1)
          ptr= nodearr.at(j);
     return ptr;

    }

    void  startSimulate()
    {
         for(int i=0  ; i < (static_cast<int> (gatearr.size())) ; i++)
              {
                 gatearr.at(i)->simulateGate();
              }
    }

    static Simulator& createobj()
    {
        static Simulator obj;
        return obj;
    }

    const vector<Node*>& getNodearr() const {
        return nodearr;
    }
};



class GateGenerator
{

private:
    Gate* PtrGate;
    Node* PtrNode;
    Simulator& c = Simulator::createobj();

    GateGenerator(const GateGenerator& other)=delete;

    void LineToWords(string str, vector<string>& line)
    {
        string word = "";

        for (auto x : str)
        {
            if (x == ' ')
            {
                line.push_back(word);
                word = "";
            }
            else {
                word = word + x;
            }
        }
        line.push_back(word);
    }



    Node* createNode(string nodename)
    {
        PtrNode=new Node(nodename);
        c.postNode(PtrNode);
        return PtrNode;
    }


    Gate* createGate(vector<string> info)
    {

    Node* ptttr=nullptr;

    if(info.at(0) != "NOT")
    {

    Node* ptrarr[3];

     for(int i=1 ; i<4 ; i++)
     {
         ptttr = c.FindNode(info.at(i));
        if(ptttr == nullptr)
        {
            ptrarr[i-1]=createNode(info.at(i));
        }
        else
        {
            ptrarr[i-1]=ptttr;
        }

     }

     PtrGate = new Gate(info.at(0),*ptrarr[0],*ptrarr[1],*ptrarr[2]);

    }
    else
    {
        Node* notarr[2];
         Node temp("");

         for(int i=1 ; i<3 ; i++)
         {
             ptttr = c.FindNode(info.at(i));
            if(ptttr == nullptr)
            {
                notarr[i-1]=createNode(info.at(i));
            }
            else
            {
                notarr[i-1]=ptttr;
            }

         }

         PtrGate = new Gate(info.at(0),*notarr[0],*notarr[1],temp);
    }

        return PtrGate;
    }

public:
    GateGenerator():PtrGate(nullptr),PtrNode(nullptr){}



    void parseInput()
    {
        string line;
        vector<string> words;
        bool value;

        while(getline(cin,line,'\n'))
        {
            LineToWords(line,words);

            if(words.at(0)=="SET")
            {

                Node* ptrr = c.FindNode(words.at(1));
                value=static_cast<bool>(stoi(words.at(2)));
                ptrr->setValue(value);

            }
            else if(words.at(0)=="SIM")
            {

                c.startSimulate();

            }
            else if(words.at(0)=="OUT")
            {
              if(words.at(1)=="ALL")
              {
                  bool x;
                  string NAME;

                  for(int i=0  ; i < (static_cast<int> (c.getNodearr().size())) ; i++)
                     {
                        x=c.getNodearr().at(i)->getValue();
                        NAME=c.getNodearr().at(i)->getName();
                        cout<<NAME<<": "<<x<<endl;
                     }
              }
              else
              {
                  Node* NodePtr = c.FindNode(words.at(1));
                  cout<< words.at(1) << ": " << NodePtr->getValue() <<endl;
              }

            }
            else
            {
                Gate* pttr=createGate(words);
                c.postGate(pttr);

            }

           words.clear();
        }

    }


    ~GateGenerator()
    {

       delete PtrGate;
       delete PtrNode;
    }


};









int main() {

GateGenerator a;
a.parseInput();


    return 0;
}
