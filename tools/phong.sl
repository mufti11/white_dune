surface phong( 
               float Ka = 0.5, Kd =0.5, Ks = 1;
               float roughness = 0.1;
               color specularcolor = 1; 
             )
{
    normal Nf = faceforward( normalize(N), I );
    vector V = -normalize(I);
    color C = 0;illuminance( P ) 
       {
       vector R = 2*normalize(N)*(normalize(N) . normalize( L )) -normalize( L );
       C += Ka*Cs + Kd*Cs*( normalize(N) . normalize(L) ) +Ks*specularcolor* pow(( R . V ), 10);
       }
    Ci = C*Cs;
}
