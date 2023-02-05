#include <iostream> 
#include <string.h>
#include <bits/stdc++.h> //for string stream
#include <vector>
#include <algorithm> //for replace, finds function

using namespace std;

//unifies 2 terms and returns a subsitution if there exists one
string unify(string term1, string term2) {

  /*
  * both terms are constant or functions or empty, 
  * if both terms are same then no change or else fail
  */
  if (islower(term1.at(0)) && islower(term2.at(0)) || term1.empty() && term2.empty())
  {
    if (term1 == term2) 
    {
      return "";
    }
    else 
    {
      return "Fail";
    }
  }

  //first term variable and is not in second term then return substitution or else return Fail
  if (isupper(term1.at(0)))
  {
    if ((term2.find(term1)) == string::npos)
    {
      return {term1 + "=" + term2};
    }
    else
    {
      return "Fail";
    }
  }

  //second term variable and is not in first term then return substitution or else return Fail
  if (isupper(term2.at(0)))
  {
    if ((term1.find(term2)) == string::npos)
    {
      return {term2 + "=" + term1};
    }
    else
    {
      return "Fail";
    }
  }

  //both terms are empty then return fail
  if (term1.empty() || term2.empty())
  {
    return "Fail";
  }

  return "Fail";
}

//removes the leading spaces before and after a string
string trim(string word)
{
  //space at start, remove 1st character
  if(word.at(0) == ' ')
  {
    word = word.substr(1);
    return trim(word);
  }
  //space at end, remove last character
  else if(word.at(word.length()-1) == ' ')
  {
    word = word.substr(0, word.length()-1);
    return trim(word);
  }
  //space at neither start nor end , return work as it is
  else
  {
    return word;
  }
}

//seperate each term from the expression and store in a vector
vector<string> seprateElem(string exp)
{
  vector<string> arr;

  //get the output stream of the string
  stringstream s(exp);
  string word, temp = "", add = "";
  int numBracks = 0;
  //continue until end of string
  while (!s.eof()) 
  {
    //get each term from the exp, terms are separated by ,
    getline(s, word, ',');
    word = trim(word);

    /*
    * if the term is a function, then add the whole function, 
    * including each term of the function as whole, 
    * start checking from the start of the bracket until the last bracket is found
    * and add whole thing as single term
    */
  
    if(word.find('(') != string::npos)
    {
      temp += word + ",";
      add = "added";
      for(int i = 0; i < word.length(); i++)
      {
        if(word.at(i) == '('){
          numBracks++;
        }
      }
    }
    
    if(word.find(')') != string::npos)
    {
      for(int i = 0; i < word.length(); i++)
      {
        if(word.at(i) == ')'){
          numBracks--;
        }
      }
      
      if(numBracks == 0)
      {
        temp += word;
        word = temp;
        temp = "";
      }
    }

    //if term complete add to vector
    if(temp == "")
    {
      arr.push_back(word);
      add = "";
    }
    else if(add != "added")
    {
      temp += word + ",";
    }
  }
  return arr;
}

//differentiates vriables, constants and functions and performs unification accordingly
string unification(string exp1, string exp2)
{
  //any one term is variable, perform unification
  if(isupper(exp1.at(0)) || isupper(exp2.at(0)))
  {  
    return unify(exp1, exp2);
  }
  else
  {
    int start1, start2, end1, end2;
    start1 = exp1.find_first_of('(');
    start2 = exp2.find_first_of('(');
    end1 = exp1.find_last_of(')');
    end2 = exp2.find_last_of(')');

    //if there is a function in the expression, split the function and get the terms
    if(start1 != string::npos && start2 != string::npos && end1 != string::npos && end2 != string::npos)
    { 
      //get the string inside the function
      string subExp1, subExp2;
      subExp1 = exp1.substr(start1 + 1, end1-start1-1);
      subExp2 = exp2.substr(start2 + 1, end2-start2-1);

      //separate each term and store in the vector
      vector<string> exp1Elements = seprateElem(subExp1);
      vector<string> exp2Elements = seprateElem(subExp2);

      string temp, result = "";
      for(int i = 0; i < exp1Elements.size(); i++){
        //unify each terms of both expressions
        temp = unification(exp1Elements[i], exp2Elements[i]);

        //unification did not fail or terms were not same
        if(temp != "" && temp != "Fail" )
        {
          //substitution set has more than 1 substitution
          if(temp.find(',') != string::npos)
          {
            //get each substition from the substitution set and store in vector
            vector<string> tempVector = seprateElem(temp);
            
            for(int j = 0; j < tempVector.size(); j++){
              //replace all occuring single variables in the exp with the substitued constant/function 
              int pos = tempVector[j].find('=');
              replace(exp1Elements.begin(), exp1Elements.end(), tempVector[j].substr(0,pos), tempVector[j].substr(pos+1));
              replace(exp2Elements.begin(), exp2Elements.end(), tempVector[j].substr(0,pos), tempVector[j].substr(pos+1)); 

              //replace all the variables in a function with susbtituted constant/function
              for(int a = 0; a < exp1Elements.size(); a++)
              {
                //get the position where the variable is found inside a function in expression 1 and replace
                int foundAt = exp1Elements[a].find(tempVector[j].substr(0,pos));
                if(foundAt != string::npos)
                {
                  exp1Elements[a] = exp1Elements[a].substr(0,foundAt) + tempVector[j].substr(pos+1) + exp1Elements[a].substr(foundAt+1);
                }

                //get the position where the variable is found inside a function in expression 2 and replace
                foundAt = exp2Elements[a].find(tempVector[j].substr(0,pos));
                if(foundAt != string::npos)
                {
                  exp2Elements[a] = exp2Elements[a].substr(0,foundAt) + tempVector[j].substr(pos+1) + exp2Elements[a].substr(foundAt+1);
                }
              }
            }
          }
          else{
            //do the same as above but with one substitution in the substitution set
            int pos = temp.find('=');
            replace(exp1Elements.begin(), exp1Elements.end(), temp.substr(0,pos), temp.substr(pos+1));
            replace(exp2Elements.begin(), exp2Elements.end(), temp.substr(0,pos), temp.substr(pos+1)); 

            for(int a = 0; a < exp1Elements.size(); a++)
            {
              int foundAt = exp1Elements[a].find(temp.substr(0,pos));
              if(foundAt != string::npos)
              {
                exp1Elements[a] = exp1Elements[a].substr(0,foundAt) + temp.substr(pos+1) + exp1Elements[a].substr(foundAt+1);
              }

              foundAt = exp2Elements[a].find(temp.substr(0,pos));
              if(foundAt != string::npos)
              {
                exp2Elements[a] = exp2Elements[a].substr(0,foundAt) + temp.substr(pos+1) + exp2Elements[a].substr(foundAt+1);
              }
            }
          }
        }
        
        if(temp == "Fail")
        {
          return temp;
        }
        else if(temp != "")
        {
          //add substitution set to the results
          result += temp + ", ";
        }
      }

      //remove last space and , from the result
      result = trim(result);
      if(result.at(result.length()-1) == ',')
      {
        result = result.substr(0, result.length()-1);
      }
      return result;
    }
    //if both are constants 
    else{
      return unify(exp1, exp2);
    }
  }
}

//ckecks if a term is variable or not by matching the specified characters
bool isVariable(string exp){
  return (exp.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789=,_()") == string::npos);
}

int main() {
  string term1, term2;
  cout << "This is an implementation of the unification algorithm." << endl;

  cout << "Please enter the first expression: ";
  cin >> term1;

  cout << "Please enter the second expression: ";
  cin >> term2;

  //not valid variable, return invalid
  if(!isVariable(term1) || !isVariable(term2))
  {
    cout << "The unifier is Invalid" << endl;
    return 0;
  }

  string unifier = unification(term1, term2);
  if(unifier == "Fail")
  {
    cout << "There is no unifier" << endl;
  }
  else{
    cout << "The unifier is: " << unifier << endl;
  }
  
  return 0;
}