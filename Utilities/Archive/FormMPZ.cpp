#include "FormMPZ.h"

static const char label[] = "FormMPZ: ";

/****************************************************************
 * Constructor.
**/
FormMPZ::FormMPZ()
{
  a = 0;
  b = 0;
  c = 0;
  disc = 0;
}
/****************************************************************
 * Destructor.
**/
FormMPZ::~FormMPZ()
{
}

/****************************************************************
 * Accessors and mutators.
**/
mpz_class FormMPZ::getA() const { return this->a; } 
void FormMPZ::setA(const mpz_class what) { this->a = what; }

mpz_class FormMPZ::getB() const { return this->b; }
void FormMPZ::setB(const mpz_class what) { this->b = what; }

mpz_class FormMPZ::getC() const { return this->c; }
void FormMPZ::setC(const mpz_class what) { this->c = what; }

mpz_class FormMPZ::getDisc() const { return this->disc; }
void FormMPZ::setDisc(const mpz_class what) { this->disc = what; }

void FormMPZ::setForm(const mpz_class newA, const mpz_class newB,
                      const mpz_class newC)
{
  this->a = newA;
  this->b = newB;
  this->c = newC;
  this->computeDisc();
}

/****************************************************************
 * General functions.
**/
/****************************************************************
 * Compute the disc from 'a', 'b', 'c'
**/
void FormMPZ::computeDisc()
{
  this->disc = (this->getB())*(this->getB()) -
                    4L*(this->getA())*(this->getC());
} // void FormMPZ::computeDisc()

/****************************************************************
 * Compound two forms.
**/
FormMPZ FormMPZ::compound(const FormMPZ& g)
{
  mpz_class locala1, localb1, localc1;
  mpz_class locala2, localb2, localc2;
  mpz_class locala3, localb3, localc3;
  mpz_class a2n, c1bya2n, m, n, t, u, w, x, y, z;
  FormMPZ* newForm;

  newForm = new FormMPZ;

  locala1 = this->getA();
  localb1 = this->getB();
  localc1 = this->getC();

  locala2 = g.getA();
  localb2 = g.getB();
  localc2 = g.getC();

// cout << endl;
// cout << "one " << " locala1=" << locala1
//                << " localb1=" << localb1
//                << " localc1=" << localc1 << endl;
// cout << "two " << " locala2=" << locala2
//                << " localb2=" << localb2
//                << " localc2=" << localc2 << endl;

  localb3 = (localb1 + localb2) / 2L;
// cout << "thr " << " localb3=" << localb3 << endl;
  if( localb3 != 0L)
  {
    Arith::euclid(locala1, localb3, x, y, m);
// cout << "fou " << " locala1=" << locala1 << " x=" << x
//                << " localb3=" << localb3 << " y=" << y
//                << " m=" << m << endl;
  }
  else
  {
    m = locala1;
    x = 1L;
    y = 0L;
// cout << "fiv " << " x=" << x << " y=" << y << " m=" << m << endl;
  }

  Arith::euclid(m, locala2, t, u, n);
// cout << "six " << " m=" << m << " t=" << t
//                << " locala2=" << locala2
//                << " u=" << u << " n=" << n << endl;
  a2n = locala2 / n;
  c1bya2n = localc1 % a2n;
// cout << "sev " << " a2n=" << a2n << " c1bya2n=" << c1bya2n << endl;
  w = ( (localb3 - localb1) * x - c1bya2n * y) % a2n;
  z = (t * w) % a2n;
// cout << "eig " << " w=" << w << " z=" << z << endl;

  locala3 = locala1 / n;
  localb3 = localb1 + 2L * locala3 * z;
  locala3 = locala3 * a2n;
  localc3 = findc(locala3, localb3, this->disc);
// cout << "nin " << " locala3=" << locala3
//                << " localb3=" << localb3
//                << " localc3=" << localc3 << endl;

  (*newForm).setA(locala3);
  (*newForm).setB(localb3);
  (*newForm).setC(localc3);
  (*newForm).setDisc(disc);

  if((*newForm).getDisc() < 0L)
  {
    (*newForm).reduceDef();
  }
  else
  {
    (*newForm).reduceInd();
  }

  return (*newForm);
} // FormMPZ& FormMPZ::compound(FormMPZ g)

/****************************************************************
 * Cycle an indefinite form to a minimal 'a' value
 * Return the number of adjacency steps taken.
**/
mpz_class FormMPZ::cycleInd()
{
//  cout << label << "enter cycle " << this->toString() << endl;

  bool donecycle;
  mpz_class locala, localb, localc;
  mpz_class absc, delta, leasta, leastb, leastc, q, r, rootdisc;
  mpz_class returnValue;

  locala = this->getA();
  localb = this->getB();
  localc = this->getC();

  donecycle = false;
  returnValue = 0;
//  rootdisc = (int) (sqrt( (double) this->disc));
  rootdisc = sqrt(this->disc);
  delta = 0L;
  q = locala;
  r = localb;
  leasta = (locala > 0L) ? locala : 1000000000L;
  leastb = localb;
  leastc = localc;
#ifdef DEBUG
  printf("   delta            (a,b,c)               least (a,b,c)\n");
  printf("%8lld--(%7lld,%7lld,%7lld)", delta, locala,localb,localc);
  printf("--(%7lld,%7lld,%7lld)\n",leasta,leastb,leastc);
#endif
  while (!donecycle)
  {
//    absc = (localc > 0L) ? localc : -localc;
    if(localc > 0)
      absc = localc;
    else
      absc = -localc;

    delta = (rootdisc + localb) / (absc + absc);
    locala = localc;
    localb = -localb + 2L * absc * delta;
    localc = findc(locala,localb,this->disc);
#ifdef DEBUG
  printf("%8lld--(%7lld,%7lld,%7lld)", delta, locala,localb,localc);
  printf("--(%7lld,%7lld,%7lld)\n",leasta,leastb,leastc);
#endif

    if( (locala == q) && (localb == r) )
    {
      donecycle = true;
    }
    else if( (locala > 0) && (locala < leasta) )
    {
      leasta = locala;
      leastb = localb;
      leastc = localc;
    }
    else if( (locala == leasta) && (localb < leastb) )
    {
      leasta = locala;
      leastb = localb;
      leastc = localc;
    }
    ++returnValue;
  }
  this->setA(leasta);
  this->setB(leastb);
  this->setC(leastc);

  return returnValue;

//  cout << label << "leave cycle " << this->toString() << endl;
} // LONG FormMPZ::cycleInd()

/****************************************************************
 * Duplicate a form.
**/
FormMPZ FormMPZ::duplicate()
{
  mpz_class locala3, localb3, localc3;
  mpz_class cbyaa, m, x, y;
  FormMPZ* newForm;

  newForm = new FormMPZ;

  if(0L == this->getB())
  {
    locala3 = 1L;
    locala3 = 0L;
    locala3 = this->getDisc() / 4L;
  }
  else
  {
    Arith::euclid(this->getA(), this->getB(), x, y, m);
    locala3 = this->getA() / m;
    cbyaa =  this->getC() % (locala3);
    localb3 = this->getB() - 2L * locala3 * ( (cbyaa * y) % locala3 );
    locala3 = locala3 * locala3;
    localc3 = findc(locala3, localb3, this->getDisc());
  }

  (*newForm).setA(locala3);
  (*newForm).setB(localb3);
  (*newForm).setC(localc3);
  (*newForm).setDisc(this->disc);

  if((*newForm).getDisc() < 0)
  {
    (*newForm).reduceDef();
  }
  else
  {
    (*newForm).reduceInd();
  }

  return (*newForm);
} // FormMPZ FormMPZ::duplicate()

/****************************************************************
 * Reduce a definite form
**/
void FormMPZ::reduceDef()
{
//  char s[80];
  mpz_class locala, localb, localc;
  mpz_class absb, c2, delta, sum;

  locala = this->getA();
  localb = this->getB();
  localc = this->getC();

//  absb = (localb > 0L) ? localb : -localb;
  if(localb > 0)
    absb = localb;
  else
    absb = -localb;
  while( (absb > locala) || (locala > localc) )
  {
    c2 = localc + localc;
    sum = localb + localc;
//    delta = (sum < 0L) ? (sum/c2 - 1L) : sum/c2;
    if(sum <  0)
      delta = sum/c2 - 1;
    else
      delta = sum/c2;

    locala = localc;
    localb = -localb + c2*delta;
    localc = findc(locala, localb, this->disc);
#ifdef DEBUGREDUCE
    printf("Form during reduction %8lld %8lld %8lld\n",locala,localb,localc);
#endif
//    absb = (localb > 0L) ? localb : -localb;
    if(localb > 0)
      absb = localb;
    else
      absb = -localb;
  }

  if( (locala == localc) && (localb < 0L) )
  {
    localb = -localb;
  }

  if( (localb < 0L) && (locala == -localb) )
  {
    localb = -localb;
  }

  this->setA(locala);
  this->setB(localb);
  this->setC(localc);
} // void FormMPZ::reduceDef()

/****************************************************************
 * Reduce an indefinite form
 * Return the number of adjacency steps taken.
**/
mpz_class FormMPZ::reduceInd()
{
  bool donereduce;
  mpz_class locala, localb, localc;
  mpz_class abs2a, abs2c, delta; // , rootdisc;
  mpz_class returnValue;
  mpz_class rootdisc;

// cout << label << "enter reduceInd" << this->toString() << endl;
  locala = this->getA();
  localb = this->getB();
  localc = this->getC();

//   printf("%8lld--(%7lld,%7lld,%7lld)\n", delta, locala, localb, localc);

  donereduce = false;
  returnValue = 0;
//  rootdisc = static_cast<LONG>(sqrt((static_cast<double>(disc))));
//  rootdisc = sqrt((static_cast<double>(disc)));
  rootdisc = sqrt(disc);
//  abs2a = (locala >= 0L) ? (locala + locala) : -(locala + locala);
  if(locala >= 0)
    abs2a = locala + locala;
  else
    abs2a = -(locala + locala);
//  abs2c = (localc >= 0L) ? (localc + localc) : -(localc + localc);
  if(localc >= 0)
    abs2c = localc + localc;
  else
    abs2c = -(localc + localc);
  if( (localb <= rootdisc) &&
      ((rootdisc - localb) < abs2a) &&
       (abs2a < (rootdisc + localb)) )
    donereduce = true;
  delta = 0L;
#ifdef DEBUG
  printf("rootdisc %lf %lld %lld\n", rootdisc, abs2a, abs2c);
  printf("   delta            (a,b,c)\n");
  printf("%8lld--(%7lld,%7lld,%7lld)\n", delta, locala, localb, localc);
#endif
  while(!donereduce)
  {
    delta = (rootdisc + localb) / abs2c;
    if (delta < 0L) delta--;
    if(localc < 0L)
    {
      delta = -delta;
    }
    localb = -localb + 2L * localc * delta;
    locala = localc;
    localc = findc(locala, localb, this->disc);
#ifdef DEBUG
    printf("%8lld--(%7lld,%7lld,%7lld)\n", delta, locala,localb,localc);
#endif
    abs2a = (locala >= 0) ? locala + locala : -(locala + locala);
    abs2c = (localc >= 0) ? localc + localc : -(localc + localc);
  if( (localb <= rootdisc) &&
      ((rootdisc - localb) < abs2a) &&
       (abs2a < (rootdisc + localb)) )
      donereduce = true;

    ++returnValue;
  }

  this->setA(locala);
  this->setB(localb);
  this->setC(localc);

// cout << "leave reduceInd " << this->toString() << endl;

  return returnValue;
} // LONG FormMPZ::reduceInd()

/****************************************************************
 * Compute 'c' from 'a', 'b', and 'disc'
**/
mpz_class FormMPZ::findc(const mpz_class a, const mpz_class b,
                         const mpz_class disc)
{
  mpz_class c;
  c = (b*b - disc) / (4L*a);
  return(c);
} // mpz_class FormMPZ::findc(mpz_class a, mpz_class b, mpz_class disc)

/****************************************************************
 * toString
**/
string FormMPZ::toString()
{
  char s[2048];
  string returnString;

//  sprintf(s, "%17lld(%12lld,%12lld,%12lld)", disc, a, b, c);
  returnString = "";
  gmp_sprintf(s,"%Zd(%Zd,%Zd,%Zd)", (this->getDisc()).get_mpz_t(),
                 (this->getA()).get_mpz_t(),
                 (this->getB()).get_mpz_t(),
                 (this->getC()).get_mpz_t());
  returnString += string(s);

//  return string(s);
  return returnString;
} // string FormMPZ::toString()

/****************************************************************
 * Overloads.
**/
/****************************************************************
 * Unary minus to return opposite.
**/
const FormMPZ FormMPZ::operator -() const
{
  FormMPZ* f1;

  f1 = new FormMPZ;
  (*f1) = (*this);
  (*f1).setB(-(*f1).getB());

  return (*f1);
} // string FormMPZ::toString()

/****************************************************************
 * EqualsEquals to test equality.
**/
bool FormMPZ::operator ==(const FormMPZ& f2) const
{
  bool returnValue;

  returnValue = false;

  if( (this->getA() == f2.getA()) &&
      (this->getB() == f2.getB()) &&
      (this->getC() == f2.getC()))
  {
    returnValue = true;
  }

  return returnValue;
} // const FormMPZ FormMPZ::operator *(const FormMPZ& f2) const

/****************************************************************
 * Multiplication to return composition. 
**/
const FormMPZ FormMPZ::operator *(const FormMPZ& f2) const
{
  FormMPZ* f1;

  f1 = new FormMPZ;
  (*f1) = (*this);
  if((*this) == f2)
  {
    return f1->duplicate();
  }
  else
  {
    return f1->compound(f2);
  }

} // const FormMPZ FormMPZ::operator *(const FormMPZ& f2) const

/****************************************************************
 * Testing routines.
**/
/****************************************************************
 * Test definite forms.
void FormMPZ::testformsdef()
{
  LONG a, b, c;
  LONG order;
  FormMPZ f1, f2, g, h;

  Utils::inStream >> a >> b >> c;
  while(!Utils::inStream.eof())
  {
    f1.setForm(a, b, c);
    if(f1.getDisc() < 0)
    {
      f1.reduceDef();
      cout << label << "form f1 " << f1.toString() << endl;
    }

    //
    // Compute order of the form directly.
    //
    order = 1L;
    g = f1;
    cout << label << "form f1 power " << order << " "
         << g.toString() << endl;
    while(g.getA() != 1L)
    {
      g = g * f1;
      ++order;
      cout << label << "form f1 power " << order << " "
           << g.toString() << endl;
    }

    //
    // Compute order of the opposite form
    //
    order = 1L;
    g = -f1;
    cout << label << "form -f1 power " << order << " "
         << g.toString() << endl;
    while(g.getA() != 1L)
    {
      g = g * (-f1);
      ++order;
      cout << label << "form -f1 power " << order << " "
           << g.toString() << endl;
    }

    cout << endl;
 
    Utils::inStream >> a >> b >> c;
  } // while(!Utils::inStream.eof())

} // void FormMPZ::testformsdef()
**/

/****************************************************************
 * Test indefinite forms.
void FormMPZ::testformsind()
{
  mpz_class a, b, c;
  mpz_class order;
  FormMPZ f1, f2, g, h;

  Utils::inStream >> a >> b >> c;
  while(!Utils::inStream.eof())
  {
    f1.setForm(a, b, c);
    if(f1.getDisc() > 0)
    {
      f1.reduceInd();
      cout << label << "form f1 reduced " << f1.toString() << endl;
      f1.cycleInd();
      cout << label << "form f1 cycled " << f1.toString() << endl;
    }

    //
    // Compute order of the form directly.
    //
    order = 1L;
    g = f1;
    cout << label << "form f1 power " << order << " "
         << g.toString() << endl;
    while(g.getA() != 1L)
    {
      g = g * f1;
      ++order;
      cout << label << "form f1 power " << order << " "
           << g.toString() << endl;
      g.cycleInd();
      cout << label << "form f1 power cycled " << order << " "
           << g.toString() << endl;
    }

    //
    // Compute order of the opposite form
    //
    order = 1L;
    g = -f1;
    cout << label << "form -f1 power " << order << " "
         << g.toString() << endl;
    g.reduceInd();
    cout << label << "form -f1 power " << order << " "
         << g.toString() << endl;
    while(g.getA() != 1L)
    {
      g = g * (-f1);
      ++order;
      cout << label << "form -f1 power " << order << " "
           << g.toString() << endl;
      g.cycleInd();
      cout << label << "form -f1 power cycled " << order << " "
           << g.toString() << endl;
    }

    cout << endl;
 
    Utils::inStream >> a >> b >> c;
  } // while(!Utils::inStream.eof())

} // void FormMPZ::testformsind()
**/

