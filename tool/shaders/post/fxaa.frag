out vec4 outTex;in vec2 texCoords;uniform sampler2D sColor;uniform vec2 viewportSize;float f(vec4 v){return v.a;}vec4 f(vec2 a,vec4 g,sampler2D t,sampler2D v,sampler2D r,vec2 m,vec4 s,vec4 o,vec4 b,float i,float e,float x,float u,float l,float d,vec4 c){vec2 L;L.r=a.r;L.g=a.g;vec4 n=textureLod(t,L,0.);float O=f(textureLodOffset(t,L,0.,ivec2(0,1))),p=f(textureLodOffset(t,L,0.,ivec2(1,0))),C=f(textureLodOffset(t,L,0.,ivec2(0,-1))),D=f(textureLodOffset(t,L,0.,ivec2(-1,0))),T=max(O,n.a),S=min(O,n.a),z=max(p,T),w=min(p,S),Z=max(C,D),Y=min(C,D),X=max(Z,z),W=min(Y,w),V=X*e,U=X-W,R=max(x,V);bool Q=U<R;if(Q)return n;float P=f(textureLodOffset(t,L,0.,ivec2(-1,-1))),N=f(textureLodOffset(t,L,0.,ivec2(1,1))),M=f(textureLodOffset(t,L,0.,ivec2(1,-1))),K=f(textureLodOffset(t,L,0.,ivec2(-1,1))),J=C+O,I=D+p,H=1./U,G=J+I,F=-2.*n.a+J,E=-2.*n.a+I,B=M+N,A=P+M,y=-2.*p+B,q=-2.*C+A,k=P+K,j=K+N,h=abs(F)*2.+abs(y),ab=abs(E)*2.+abs(q),ac=-2.*D+k,ad=-2.*O+j,ae=abs(ac)+h,af=abs(ad)+ab,ag=k+B,ah=m.r;bool ai=ae>=af;float aj=G*2.+ag;if(!ai)C=D;if(!ai)O=p;if(ai)ah=m.g;float ak=aj*(1./12.)-n.a,al=C-n.a,am=O-n.a,an=C+n.a,ao=O+n.a;bool ap=abs(al)>=abs(am);float aq=max(abs(al),abs(am));if(ap)ah=-ah;float ar=clamp(abs(ak)*H,0.,1.);vec2 as;as.r=L.r;as.g=L.g;vec2 at;at.r=!ai?0.:m.r;at.g=ai?0.:m.g;if(!ai)as.r+=ah*.5;if(ai)as.g+=ah*.5;vec2 au;au.r=as.r-at.r;au.g=as.g-at.g;vec2 av;av.r=as.r+at.r;av.g=as.g+at.g;float aw=-2.*ar+3.,ax=f(textureLod(t,au,0.)),ay=ar*ar,az=f(textureLod(t,av,0.));if(!ap)an=ao;float ba=aq/4.,bb=n.a-an*.5,bc=aw*ay;bool bd=bb<0.;ax-=an*.5;az-=an*.5;bool be=abs(ax)>=ba,bf=abs(az)>=ba;if(!be)au.r-=at.r;if(!be)au.g-=at.g;bool bg=!be||!bf;if(!bf)av.r+=at.r;if(!bf)av.g+=at.g;if(bg){if(!be)ax=f(textureLod(t,au.rg,0.));if(!bf)az=f(textureLod(t,av.rg,0.));if(!be)ax=ax-an*.5;if(!bf)az=az-an*.5;be=abs(ax)>=ba;bf=abs(az)>=ba;if(!be)au.r-=at.r;if(!be)au.g-=at.g;bg=!be||!bf;if(!bf)av.r+=at.r;if(!bf)av.g+=at.g;if(bg){if(!be)ax=f(textureLod(t,au.rg,0.));if(!bf)az=f(textureLod(t,av.rg,0.));if(!be)ax=ax-an*.5;if(!bf)az=az-an*.5;be=abs(ax)>=ba;bf=abs(az)>=ba;if(!be)au.r-=at.r;if(!be)au.g-=at.g;bg=!be||!bf;if(!bf)av.r+=at.r;if(!bf)av.g+=at.g;if(bg){if(!be)ax=f(textureLod(t,au.rg,0.));if(!bf)az=f(textureLod(t,av.rg,0.));if(!be)ax=ax-an*.5;if(!bf)az=az-an*.5;be=abs(ax)>=ba;bf=abs(az)>=ba;if(!be)au.r-=at.r;if(!be)au.g-=at.g;bg=!be||!bf;if(!bf)av.r+=at.r;if(!bf)av.g+=at.g;if(bg){if(!be)ax=f(textureLod(t,au.rg,0.));if(!bf)az=f(textureLod(t,av.rg,0.));if(!be)ax=ax-an*.5;if(!bf)az=az-an*.5;be=abs(ax)>=ba;bf=abs(az)>=ba;if(!be)au.r-=at.r*1.5;if(!be)au.g-=at.g*1.5;bg=!be||!bf;if(!bf)av.r+=at.r*1.5;if(!bf)av.g+=at.g*1.5;if(bg){if(!be)ax=f(textureLod(t,au.rg,0.));if(!bf)az=f(textureLod(t,av.rg,0.));if(!be)ax=ax-an*.5;if(!bf)az=az-an*.5;be=abs(ax)>=ba;bf=abs(az)>=ba;if(!be)au.r-=at.r*2.;if(!be)au.g-=at.g*2.;bg=!be||!bf;if(!bf)av.r+=at.r*2.;if(!bf)av.g+=at.g*2.;if(bg){if(!be)ax=f(textureLod(t,au.rg,0.));if(!bf)az=f(textureLod(t,av.rg,0.));if(!be)ax=ax-an*.5;if(!bf)az=az-an*.5;be=abs(ax)>=ba;bf=abs(az)>=ba;if(!be)au.r-=at.r*2.;if(!be)au.g-=at.g*2.;bg=!be||!bf;if(!bf)av.r+=at.r*2.;if(!bf)av.g+=at.g*2.;if(bg){if(!be)ax=f(textureLod(t,au.rg,0.));if(!bf)az=f(textureLod(t,av.rg,0.));if(!be)ax=ax-an*.5;if(!bf)az=az-an*.5;be=abs(ax)>=ba;bf=abs(az)>=ba;if(!be)au.r-=at.r*2.;if(!be)au.g-=at.g*2.;bg=!be||!bf;if(!bf)av.r+=at.r*2.;if(!bf)av.g+=at.g*2.;if(bg){if(!be)ax=f(textureLod(t,au.rg,0.));if(!bf)az=f(textureLod(t,av.rg,0.));if(!be)ax=ax-an*.5;if(!bf)az=az-an*.5;be=abs(ax)>=ba;bf=abs(az)>=ba;if(!be)au.r-=at.r*2.;if(!be)au.g-=at.g*2.;bg=!be||!bf;if(!bf)av.r+=at.r*2.;if(!bf)av.g+=at.g*2.;if(bg){if(!be)ax=f(textureLod(t,au.rg,0.));if(!bf)az=f(textureLod(t,av.rg,0.));if(!be)ax=ax-an*.5;if(!bf)az=az-an*.5;be=abs(ax)>=ba;bf=abs(az)>=ba;if(!be)au.r-=at.r*4.;if(!be)au.g-=at.g*4.;bg=!be||!bf;if(!bf)av.r+=at.r*4.;if(!bf)av.g+=at.g*4.;if(bg){if(!be)ax=f(textureLod(t,au.rg,0.));if(!bf)az=f(textureLod(t,av.rg,0.));if(!be)ax=ax-an*.5;if(!bf)az=az-an*.5;be=abs(ax)>=ba;bf=abs(az)>=ba;if(!be)au.r-=at.r*8.;if(!be)au.g-=at.g*8.;bg=!be||!bf;if(!bf)av.r+=at.r*8.;if(!bf)av.g+=at.g*8.;}}}}}}}}}}float bh=L.r-au.r,bi=av.r-L.r;if(!ai)bh=L.g-au.g;if(!ai)bi=av.g-L.g;bool bj=ax<0.!=bd;float bk=bi+bh;bool bl=az<0.!=bd;float bm=1./bk;bool bn=bh<bi;float bo=min(bh,bi);bool bp=bn?bj:bl;float bq=bc*bc,br=bo*-bm+.5,bs=bq*i,bt=bp?br:0.,bu=max(bt,bs);if(!ai)L.r+=bu*ah;if(ai)L.g+=bu*ah;return vec4(textureLod(t,L,0.).rgb,n.a);}void main(){vec2 a=vec2(1.)/viewportSize;outTex=f(texCoords,vec4(0),sColor,sColor,sColor,a,vec4(0),vec4(0),vec4(0),.75,.166,.0833,0.,0.,0.,vec4(0));}