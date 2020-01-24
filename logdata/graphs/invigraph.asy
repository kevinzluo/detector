//https://artofproblemsolving.com/texer/tseyxejm
size(10cm, 10cm, IgnoreAspect);
real[] x = {249, 955, 429, 698, 827, 743, 623, 679, 483, 885};
real[] y = {272.95, 370.65, 290.35, 316.55, 334.55, 321.05, 308.5, 314.1, 295.6, 346.5};

for(int i = 0; i < x.length; ++i) {
	dot((x[i], y[i]), red);
}

Label f; 
f.p=fontsize(6); 
xaxis("Sensor Reading", YEquals(260), 0,1000, Ticks(f, 100.0)); 
yaxis("Temperature (K)", LeftRight, 260,400,Ticks(f, 10.0, extend=true)); 
for(int i = 1; i <= 10; ++i) {
	draw((i * 100, 260) -- (i * 100, 400));
}
real g(real x) 
{ 
real A, B, C;
A = 0.00122964518209397;
B = 0.000214748411193937;
C = 0.000000191714874203215;
real res = 10000.0 * (1024.0 / x - 1);
real lnr = log(res);
return 1 / ( A + B * lnr + C * lnr^3 ); 
} 
guide p; 
int n = 1; 
for(int i = 1; i < 1024 * n; ++i) {
	pair q = (i / n, g(i / n));
	if(q.y <= 400 && q.y >= 260) {
		p = p..q;
    }
}
draw(p);

