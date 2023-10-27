#include <functional>
#include <iostream>
#include <cmath>

using namespace std;

int oct(int val){
  int n = 8;
  val %= n;
  if(val<0){
    val = n+val;
  }
  return val;
}

struct Digit{
  int val = 0;
  Digit* left = nullptr;
  Digit* right = nullptr;
};


Digit* newDigit(int n=0){
  Digit* digit = new Digit;
  int temp = oct(n);
  digit->val = temp;
  if(temp<n){
    digit->left = newDigit(1);
    digit->left->right = digit;
  }
  return digit;
}

void show(Digit* d){
  if(d->left)
    show(d->left);
  cout << d->val << " ";
}

int compare(Digit* d1, Digit* d2){
	if(d1->val > d2->val) return 1;
	else if(d1->val < d2->val) return -1;
	return 0;
}

Digit* lShift(Digit *d){
  Digit* td = newDigit();
  d->right = newDigit();
  d->right->left = d;
  cout << "post-shift: ";
  if(d->left) cout << d->left->val;
  cout << d->val << d->right->val << endl;
  cout << "post-shift: ";
  
  
	return d->right;
}

Digit* add(Digit* d1, Digit* d2){
  Digit* d = newDigit(0);
  if(!d1 && d2) return d2;
  if(!d2 && d1) return d1;
  int sum = oct(d1->val+d2->val);
  d->val = sum;
  if(sum<d1->val || sum < d2->val){
    if(d->left){
      d->left = add(d1->left, newDigit(1));
    }
    else
      d->left = newDigit(1);
  }
  if(d1->left || d2->left){
    if(d->left){
      d->left = add(d->left, add(d1->left, d2->left));
    }
    else {
      d->left = add(d1->left, d2->left);
    }
  }
  if(d->left)  
    d->left->right = d;
  return d;
}

//assume d1 > d2
Digit* sub(Digit* d1, Digit* d2){
	Digit* d = newDigit(0);
  if(!d2){
    d = d1;
    return d;
  }
	int diff = oct(d1->val-d2->val);
	d = newDigit(diff);
	if(d1->left){
		d->left = sub(d1->left, d2->left);
		d->left->right = d;
    if(d2->val>d1->val)
      d->left->val -= 1;
	}
  if(d->left)
    if(d->left->val == 0)
      d->left = nullptr;
	return d;
}

Digit* mult(Digit* d1, Digit* d2){
	Digit* mind = compare(d1, d2) == -1 ? d1 : d2; 
	Digit* maxd = compare(d1, d2) != -1 ? d1 : d2; 
  if(mind->val <= 1) return newDigit(d1->val*d2->val);
	Digit* d = newDigit(0);
	Digit* a = newDigit(7/mind->val); 
	Digit* b = newDigit(maxd->val/a->val); 
	if(b->val == 0){
		d->val = mind->val*maxd->val;
		return d;
	}
	d->left = b;
	Digit* temp = newDigit(7%mind->val+1);
	d = sub(d, mult(temp, b));
	a = newDigit(7/mind->val);
	b = newDigit(maxd->val/a->val);
	temp = sub(maxd,  mult(a, b));
  d = add(d, mult(temp, mind));
  cout << "d: ";
  show(d);
  cout <<endl;
	return d;
}

Digit* multNum(Digit* d1, Digit* d2){
  function<Digit* (Digit*, Digit*)> func = 
	  [&func](Digit* d1, Digit* d2){
	  Digit* d = newDigit();
	  d = mult(d1, d2);
    cout << "d-f: ";
    show(d);
    cout << endl;
	  if(d1->left){
		// d = lShift(d);
      Digit* temp = newDigit();
      temp = func(d1->left, d2);
      temp = lShift(temp);
      d = add(d, temp);
      cout << "here-d-f: ";
      show(d);
      cout << endl;
      }
	  return d;
  };
  function<Digit* (Digit*, Digit*)> map = 
	  [&map, &func](Digit* d1, Digit* d2){
	  Digit* d;
	  d = func(d1, d2);
    cout << "d-m: ";
    show(d);
    cout << endl;
	  if(d2->left){
		  // d = lShift(d);
      cout << "here-m: ";
		  d = add(d, lShift(map(d1, d2->left)));
	  }
	  return d;
  };
  return map(d1, d2);
}

int main(){
  Digit* n1 = newDigit(10);
  Digit* n2 = newDigit(10);
  Digit* res;
  cout << "n1: ";
  show(n1);
  cout << endl;
  cout << "n2: ";
  show(n2);
  cout << endl;
  res = multNum(n1, n2);
  cout << "res: ";
  show(res);
  cout << endl;
  // show(newNum(mult(newDigit(1), newDigit(2))));
  return 0;
}
