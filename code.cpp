/*
    @author Mayank Kumar(19CS30029)
    FLAT Programming Assignment
*/

#include<bits/stdc++.h>
using namespace std;

#define fori(i, a, b) for(i=a; i<b; ++i)
#define fill(a, x) memset(a, x, sizeof(a))

class SET
{
private:
    unsigned int size_;
public:
    unsigned int *states;
    void setSize(unsigned int x = 0U);
    unsigned int getSize();
    SET();
    SET(unsigned int x);
    ~SET();
    bool isPresent(unsigned int x);
    void insert(unsigned int x);
};

class NFA
{
private:
    unsigned int Q, sigma;
public:
    unsigned int S, F;
    unsigned int **delta;
    void setQ(unsigned int x);
    void setSigma(unsigned int x);
    unsigned int getQ();
    unsigned int getSigma();
    NFA(unsigned int a, unsigned int b);
    ~NFA();
    friend ostream &operator<<(ostream &, NFA *);
};

class DFA
{
private:
    unsigned int Q, sigma;
public:
    unsigned int S;
    unsigned int **delta;
    SET F;
    void setQ(unsigned int x);
    void setSigma(unsigned int x);
    unsigned int getQ();
    unsigned int getSigma();
    DFA(unsigned int a, unsigned int b);
    ~DFA();
    friend ostream &operator<<(ostream &, DFA *);
};

class Node
{
public:
    Node *next;
    unsigned int x;
};

class Stack
{
    Node *HEAD;
    int size_;
public:
    void setSize(int x = -1U);
    int getSize();
    Stack();
    bool isEmpty();
    void push(unsigned int element);
    unsigned int pop();
};

void SET::setSize(unsigned int x)
{
    size_ = x;
    return;
}

unsigned int SET::getSize()
{
    return size_;
}

SET::SET()
{
    setSize();
}

SET::SET(unsigned int x)
{
    setSize();
    states = new unsigned int[x]{0};
}

SET::~SET()
{
    delete[] states;
}

bool SET::isPresent(unsigned int x)
{
    if(states[x/32U] & (1U<<(x%32)))
        return true;
    return false;
}

void SET::insert(unsigned int x)
{
    unsigned int i;
    if(isPresent(x))
        return;
    states[x/32U] |= (1U << (x%32U));
    size_++;
    return;
}

void NFA::setQ(unsigned int x)
{
    Q = x;
    return;
}

void NFA::setSigma(unsigned int x)
{
    sigma = x;
    return;
}

unsigned int NFA::getQ()
{
    return Q;
}

unsigned int NFA::getSigma()
{
    return sigma;
}

NFA::NFA(unsigned int a, unsigned int b)
{
    setQ(a);
    setSigma(b);
    delta = new unsigned int *[Q];
    unsigned int i = 0, j = 0;
    fori(i, 0, Q)
        delta[i] = new unsigned int[sigma]{0};
}

NFA::~NFA()
{
    for(unsigned int i=0; i<Q; i++)
        delete[] delta[i];
    delete[] delta;
}

ostream &operator<<(ostream &os, NFA *x)
{
    unsigned int i, p, a, q;
    os << "    Number of states: " << x->getQ() << endl;
    os << "    Input alphabet: {";
    fori(i, 0, x->getSigma()-1)
        os << i << ",";
    os << x->getSigma()-1 << "}" << endl;

    os << "    Start states: {";
    int flag = 0, k;
    fori(i, 0, x->getQ())
    {
        k = x->S & (1U << i);
        if(k==0)
            continue;
        if(flag == 0)
            os << i, flag = 1;
        else
            os << "," << i;
    }
    os << "}" << endl;
    os << "    Final states: {";
    flag = 0;
    fori(i, 0, x->getQ())
    {
        k = x->F & (1U << i);
        if(k==0)
            continue;
        if(flag == 0)
            os << i, flag = 1;
        else
            os << "," << i;
    }
    os << "}" << endl;
    os << "    Transition function" << endl;
    fori(p, 0, x->getQ())
    {
        fori(a, 0, x->getSigma())
        {
            os << "        Delta(" << p << "," << a << ") = {";
            flag = 0;
            fori(q, 0, x->getQ())
            {
                k = x->delta[p][a] & (1U << q);
                if(k==0)
                    continue;
                if(flag==0)
                    os << q, flag = 1;
                else
                    os << ","<<q;
            }
            os <<"}" << endl;
        }
    }
    return os;
}

void DFA::setQ(unsigned int x)
{
    Q = x;
    return;
}

void DFA::setSigma(unsigned int x)
{
    sigma = x;
    return;
}

unsigned int DFA::getQ()
{
    return Q;
}

unsigned int DFA::getSigma()
{
    return sigma;
}

DFA::DFA(unsigned int a, unsigned int b)
{
    setQ(a);
    setSigma(b);
    F.states = new unsigned int[(Q/32U)+((Q%32U)!=0)]{0};
    delta = new unsigned int *[Q];
    unsigned int i = 0, j = 0;
    fori(i, 0, Q)
        delta[i] = new unsigned int[sigma]{0};
}

DFA::~DFA()
{
    for(unsigned int i=0; i<Q; i++)
        delete[] delta[i];
    delete[] delta;
}

ostream &operator<<(ostream &os, DFA *x)
{
    os << "    Number of states: " << x->getQ() << endl;
    os << "    Input alphabet: {";
    unsigned int i, a, p;
    bool ok = false;
    fori(i, 0, x->getSigma() - 1)
        os << i << ",";
    os << x->getSigma() - 1 << "}" << endl;
    os << "    Start state: " << x->S << endl;
    unsigned int sz = x->F.getSize();
    if(sz < 64)
    {
        os << "    Final states: {";
        fori(i, 0, x->getQ())
        {
            if(x->F.isPresent((i)) && ok==false)
                os << i, ok = true;
            else if(x->F.isPresent((i)))
                os << "," << i;
        }
        os << "}" << endl;
    }
    else
        os << "    " << sz <<" final states" << endl;
    if(x->getQ() > 64)
    {
        os << "    Transition function: Skipped\n";
        return os;
    }
    os << "    Transition function" << endl;
    os << "    a\\p|";
    fori(i, 0, x->getQ())
    {
        if(i < 10)
            os << " ";
        os <<" " << i;
    }
    os << endl;
    os << "    ---+";
    fori(i, 0, x->getQ())
        os <<"---";
    os << endl;
    fori(a, 0, x->getSigma())
    {
        os << "     " << a << " |";
        fori(p, 0, x->getQ())
        {
            if(x->delta[p][a] < 10)
                os << " ";
            os << " " << x->delta[p][a];
        }
        os << endl;
    }
    return os;
}

void Stack::setSize(int x)
{
    size_ = x;
    return;
}

int Stack::getSize()
{
    return size_;
}

Stack::Stack()
{
    setSize();
}

bool Stack::isEmpty()
{
    if(size_ == -1)
        return true;
    return false;
}

void Stack::push(unsigned int element)
{
    if(isEmpty())
    {
        HEAD = new Node;
        HEAD->x = element;
        HEAD->next = nullptr;
    }
    else
    {
        Node *temp = HEAD;
        HEAD = new Node;
        HEAD->x = element;
        HEAD->next = temp;
    }
    size_++;
    return;
}

unsigned int Stack::pop()
{
    if(isEmpty())
    {
        cout << "Empty stack" << endl;
        exit(1);
    }
    else
    {
        unsigned int res = HEAD->x;
        Node *temp = HEAD;
        HEAD = HEAD->next;
        delete temp;
        size_--;
        return res;
    }
}

NFA *readNFA(const char *filename)
{
    ifstream file(filename);
    unsigned int n, m;
    int x;
    file >> n;
    file >> m;
    NFA *nfa = new NFA(n, m);
    file >> x;
    while(x != -1)
    {
        nfa->S |= (1U<<x);
        file >> x;
    }
    file >> x;
    while(x != -1)
    {
        nfa->F |= (1U << x);
        file >> x;
    }
    int p, a, q;
    file >> p;
    while(p != -1)
    {
        file >> a >> q;
        nfa->delta[p][a] |= (1U << q);
        file >> p;
    }
    return nfa;
}

void printNFA(NFA *x)
{
    cout << x;
    return;
}

DFA *subsetcons(NFA *nfa)
{
    unsigned int cnt;
    unsigned int Q = nfa->getQ();
    unsigned int sigma = nfa->getSigma();
    unsigned int F = nfa->F;
    unsigned int q = (1U<<Q);
    unsigned int i, j, k, l;
    unsigned int arr[32];
    fill(arr, 0);
    bool ok;
    DFA *dfa = new DFA(q, sigma);
    dfa->S = nfa->S;
    cnt = 0;
    fori(i, 0, Q)
    {
        k = F & (1U<<i);
        if(k==0)
            continue;
        arr[cnt++] = i;
    }
    fori(i, 0, q)
    {
        ok = false;
        fori(j, 0, cnt)
        {
            k = i & (1U<<arr[j]);
            if(k==0)
                continue;
            ok = true;
            break;
        }
        if(ok)
            dfa->F.insert(i);
    }
    fori(i, 0, q)
    {
        fori(j, 0, Q)
        {
            k = i & (1U<<j);
            if(k==0)
                continue;
            fori(l, 0, sigma)
            {
                unsigned int dorn = (dfa->delta[i][l]) | (nfa->delta[j][l]);
                dfa->delta[i][l] = dorn;
            }
        }
    }
    return dfa;
}

void printDFA(DFA *x)
{
    cout << x;
    return;
}

unsigned int binSearch(unsigned int *arr, int l, int r, unsigned int x)
{
    if(r<l)
        exit(1);
    int mid = r - (r-l)/2;
    if(arr[mid]==x)
        return mid;
    else if(arr[mid] > x)
        return binSearch(arr, l, mid-1, x);
    else
        return binSearch(arr, mid+1, r, x);
}

void merge(unsigned int arr[], int l, int m, int r)
{
    int n1 = m-l+1;
    int n2 = r-m;
    int i, j, k;
    unsigned int L[n1], R[n2];
    fori(i, 0, n1)
        L[i] = arr[l + i];
    fori(j, 0, n2)
        R[j] = arr[m + 1 + j];
    i = 0, j = 0, k = l;
    while (i<n1 && j<n2)
    {
        if(L[i]<=R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while(i<n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
    return;
}

void mergeSort(unsigned arr[], int l, int r)
{
    if(r<=l)
        return;
    int mid = l + (r-l)/2;
    mergeSort(arr, l, mid);
    mergeSort(arr, mid+1, r);
    merge(arr, l, mid, r);
    return;
}

void printreachable(SET *t, DFA *x)
{
    unsigned int i;
    bool ok = false;
    cout << "{";
    fori(i, 0, x->getQ())
    {
        if(t->isPresent((i)) && ok==false)
            cout << i, ok = true;
        else if(t->isPresent((i)))
            cout << "," << i;
    }
    cout << "}" << endl;
}

SET *findreachable(DFA *d)
{
    unsigned int sz = ((d->getQ())/32U)+1;
    SET *visited = new SET(sz);
    unsigned int i, curr;
    Stack x;
    x.push(d->S);
    visited->insert(d->S);
    while(!x.isEmpty())
    {
        curr = x.pop();
        fori(i, 0, d->getSigma())
        {
            if(visited->isPresent(d->delta[curr][i]))
                continue;
            visited->insert(d->delta[curr][i]), x.push(d->delta[curr][i]);
        }
    }
    // mergeSort(visited->states, 0, visited->getSize() - 1);
    return visited;
}

DFA *rmunreachable(DFA *D, SET *R)
{
    unsigned int szR = R->getSize();
    unsigned int qD = D->getQ();
    unsigned int sigmaD = D->getSigma();
    DFA *newD = new DFA(szR, sigmaD);    
    unsigned int cnt = 0;
    unsigned int arr[szR];
    fill(arr, 0);
    unsigned int i, j;
    fori(i, 0, qD)
    {
        if(!R->isPresent(i))
            continue;
        arr[cnt++] = i;
        if(D->S == i)
            newD->S = cnt-1;
        if(D->F.isPresent(i))
            newD->F.insert(cnt-1);
    }

    // mergeSort(arr, 0, R->getSize()-1);
    cnt = 0;
    fori(i, 0, qD)
    {
        if(!R->isPresent(i))
            continue;
        fori(j, 0, sigmaD)
        {
            unsigned int search = binSearch(arr, 0, szR-1, D->delta[i][j]);
            newD->delta[cnt][j] = search;
        }
        cnt++;
    }
    delete D;
    return newD;
}

unsigned int **findequiv(DFA *D)
{
    unsigned int Q = D->getQ();
    unsigned int **M  = new unsigned int *[Q];
    unsigned int a, i, j, p, p_, q, q_;
    bool ok, brk;
    fori(i, 0, Q)
        M[i] = new unsigned int[Q]{0};
    fori(i, 0, Q)
        fori(j, i+1, Q)
        {
            M[i][j] = (D->F.isPresent(i) && !(D->F.isPresent(j))) ? 1 : 0;
            M[i][j] |= (D->F.isPresent(j) && !(D->F.isPresent(i))) ? 1 : 0;
        }
    ok = true, brk = false;
    while(ok)
    {
        ok = false;
        fori(p, 0, Q)
            fori(q, p+1, Q)
            {
                if(M[p][q] == 1)
                    continue;
                else if(M[p][q] !=1)
                {
                    fori(a, 0, D->getSigma())
                    {
                        brk = false;
                        p_ = D->delta[p][a], q_ = D->delta[q][a];
                        if(M[p_][q_]==1)
                            M[p][q] = 1, ok = true, brk = true;
                        else if(M[q_][p_]==1)
                            M[p][q] = 1, ok = true, brk = true;
                        if(brk)
                            break;
                    }
                }
            }
    }
    return M;
}

void printequiv(unsigned int **M, unsigned int Q)
{
    unsigned int i, j;
    unsigned int visited[Q] = {0};
    unsigned int grpno = 0;
    fori(i, 0, Q)
    {
        unsigned int grpmem[Q] = {0}, grpsz = 0;
        fori(j, 0, Q)
        {
            if(M[i][j]==0 && visited[j]==0)
                visited[j] = 1, grpmem[grpsz++] = j;
        }
        if(grpsz>0)
        {
            cout << "    Group ";
            if(grpno < 10)
                cout << " ";
            cout << grpno << ": {";
            fori(j, 0, grpsz-1)
                cout << grpmem[j] << ",";
            cout << grpmem[j] << "}" << endl;
            grpno++;
        }
    }
}

DFA *collapse(DFA *D, unsigned int **M)
{
    unsigned int Q = D->getQ();
    unsigned int sigma = D->getSigma();
    SET S;
    S.states = new unsigned int[(Q/32U)+1]{0};
    unsigned int check[Q];
    unsigned int a, i, j, p;
    int count = 0;
    fori(i, 0, Q)
        check[i] = i;
    fori(i, 0, Q)
        fori(j, i+1, Q)
            if(M[i][j] == 0)
                check[j] = check[i];
    printequiv(M, Q);
    unsigned int arr[Q];
    fill(arr, 0);
    fori(i, 0, Q)
        S.insert(check[i]);
    fori(i, 0, Q)
        if(S.isPresent(i))
            arr[i] = count++;
    DFA *newD = new DFA(S.getSize(), sigma);
    newD->S = arr[check[D->S]];
    fori(i, 0, Q)
        if(D->F.isPresent(i))
            newD->F.insert(arr[check[i]]);
    fori(p, 0, Q)
    {
        fori(a, 0, sigma)
        {
            unsigned int p1 = arr[check[p]];
            unsigned int dt = D->delta[p][a];
            unsigned int c = check[dt];
            newD->delta[p1][a] = arr[c];
        }
    }
    delete D;
    return newD;
}

int main()
{
    char file[100];
    cout << "Enter Relative File Path: " << endl;
    cin >> file;

    cout << endl;
    
    NFA *x = readNFA(file);
    cout << "+++ Input NFA" << endl;
    printNFA(x);
    cout << endl;

    DFA *y = subsetcons(x);
    cout << "+++ Converted DFA" << endl;
    printDFA(y);
    cout << endl;

    SET *t = findreachable(y);
    cout << "+++ Reachable states: ";
    printreachable(t, y);
    cout << endl;

    DFA *y_ = rmunreachable(y, t);
    cout << "+++ Reduced DFA after removing unreachable states" << endl;
    printDFA(y_);
    cout << endl;

    unsigned int **M = findequiv(y_);
    cout << "+++ Equivalent states" << endl;
    DFA *y__ = collapse(y_, M);
    cout << endl;

    cout << "+++ Reduced DFA after collapsing equivalent states" << endl;
    printDFA(y__);
    cout << endl;
    return 0;
}
