//
//  psychometry.h
//  Psychometry
//
//  Created by Yassin Kortam on 10/31/21.
//

#include <cmath>

using namespace std;

double pws(double temp);
double w(double temp, double humidity);
double ws(double temp, double humidity);
double v(double temp, double humidity);
double p(double temp, double humidity);
double h(double temp, double humidity);
double td(double temp, double humidity);
double twb(double temp, double humidity);

double pws(double temp){
    double t = temp + 273.15, pws;
    double const c8 = -5.8002206E+03, c9 = 1.3914993E+00, c10 = -4.8640239E-02, c11 = 4.1764768E-05, c12 = -1.4452093E-08, c13 = 6.5459673E+00;
    pws = 0.001*exp((c8/t) + c9 + c10*t + c11*pow(t,2) + c12*pow(t,3) + c13*log(t));
    return pws;
}

double w(double temp, double humidity){
    double pw = pws(temp)*humidity, w;
    w = 0.622*(pw/(101.325-pw));
    return w;
}

double ws(double temp, double humidity){
    double ws = 0.622*(pws(temp)/(101.325-pws(temp)));
    return ws;
}

double v(double temp, double humidity){
    double wloc = w(temp, humidity), v;
    v = (0.287042*(temp + 273.15)*(1+1.6078*wloc))/101.325;
    return v;
}

double p(double temp, double humidity){
    double wloc = w(temp, humidity),vloc = v(temp, humidity), p;
    p = (1/vloc)*(1+wloc);
    return p;
}

double h(double temp, double humidity){
    double wloc = w(temp, humidity), h;
    h = 1.006*temp + wloc*(2501 + 1.86*temp);
    return h;
}

double td(double temp, double humidity){
    double pw = pws(temp)*humidity, a = log(pw), td;
    double const c14 = 6.54, c15 = 14.526, c16 = 0.7389, c17 = 0.09486, c18 = 0.4569;
    td = c14 + c15*a + c16*pow(a,2) + c17*pow(a,3) + c18*pow(pw, 0.1984);
    return td;
}

double twb(double temp, double humidity){
    double W = 0.01065, Ws, T = temp, phi= humidity, Twb, P = 101325, Pw, Pws;
    double c8=-5.800220e+03, c9=1.3914993, c10=-4.8640239e-2, c11=4.1764768e-5;
    double c12=-1.4452093e-8, c13=6.5459673, lnPws, lnPwsWb, PwWb, PwsWb, WsWb;

    double Tabs= T+273.15, Twbabs, Wcalc;
    // Assume Twb= T;
    Twb=T;
    Twbabs=Twb+273.15;
    while(1)
    {
    //lnPws = c8/Tabs + c9+c10*Tabs+c11*Tabs*Tabs+c12*Tabs*Tabs*Tabs+c13*log(Tabs);
    //Pws= exp(lnPws);
    lnPwsWb = c8/Twbabs + c9+c10*Twbabs+c11*Twbabs*Twbabs+c12*Twbabs*Twbabs*Twbabs+c13*log(Twbabs);
    PwsWb= exp(lnPwsWb);
    //Pw=phi*Pws;
    PwWb= phi*PwsWb;
    WsWb= 0.622*PwsWb/(P-PwsWb);
    //Ws= 0.622*Pws/(P-Pws);
    Wcalc= ((2501-2.326*Twb)*WsWb-1.006*(T-Twb))/(2501+1.86*T-4.186*Twb);
    if( fabs(Wcalc - W)<0.00005) break;
    Twb=Twb-0.08;
    Twbabs=Twb+273.15;

    }
    return Twb;
}
