#include "eqns_cat.h"
#include <cmath>

double fx(double t)
{
    return ((2337 * cos(t)) / 8. - (43 * cos(2 * t)) / 5. + (322 * cos(3 * t)) / 5. - 
            (117 * cos(4 * t)) / 5. - (26 * cos(5 * t)) / 5. - (23 * cos(6 * t)) / 3. + 
            (143 * cos(7 * t)) / 4. - (11 * cos(8 * t)) / 4. - (31 * cos(9 * t)) / 3. - 
            (13 * cos(10 * t)) / 4. - (9 * cos(11 * t)) / 2. + (41 * cos(12 * t)) / 20. + 
            8 * cos(13 * t) + (2 * cos(14 * t)) / 3. + 6 * cos(15 * t) + (17 * cos(16 * t)) / 4. - 
            (3 * cos(17 * t)) / 2. - (29 * cos(18 * t)) / 10. + (11 * cos(19 * t)) / 6. + 
            (12 * cos(20 * t)) / 5. + (3 * cos(21 * t)) / 2. + (11 * cos(22 * t)) / 12. - 
            (4 * cos(23 * t)) / 5. + cos(24 * t) + (17 * cos(25 * t)) / 8. - (7 * cos(26 * t)) / 2. - 
            (5 * cos(27 * t)) / 6. - (11 * cos(28 * t)) / 10. + cos(29 * t) / 2. - 
            cos(30 * t) / 5. - 
            (721 * sin(t)) / 4. + (196 * sin(2 * t)) / 3. - (86 * sin(3 * t)) / 3. - 
            (131 * sin(4 * t)) / 2. - (477 * sin(5 * t)) / 14. + 27 * sin(6 * t) -   
            (29 * sin(7 * t)) / 2. + (68 * sin(8 * t)) / 5. + sin(9 * t) / 10. + 
            (23 * sin(10 * t)) / 4. - (19 * sin(12 * t)) / 2. - (85 * sin(13 * t)) / 21. + 
            (2 * sin(14 * t)) / 3. + (27 * sin(15 * t)) / 5. + (7 * sin(16 * t)) / 4. + 
            (17 * sin(17 * t)) / 9. - 4 * sin(18 * t) - sin(19 * t) / 2. + sin(20 * t) / 6. + 
            (6 * sin(21 * t)) / 7. - sin(22 * t) / 8. + sin(23 * t) / 3. + (3 * sin(24 * t)) / 2. + 
            (13 * sin(25 * t)) / 5. + sin(26 * t) - 2 * sin(27 * t) + (3 * sin(28 * t)) / 5. - 
            sin(29 * t) / 5. + sin(30 * t) / 5.) / 500.0;
}

double fy(double t) {
    return ((-125 * cos(t)) / 2. - (521 * cos(2 * t)) / 9. - (359 * cos(3 * t)) / 3. + 
            (47 * cos(4 * t)) / 3. - (33 * cos(5 * t)) / 2. - (5 * cos(6 * t)) / 4. + 
            (31 * cos(7 * t)) / 8. + (9 * cos(8 * t)) / 10. - (119 * cos(9 * t)) / 4. - 
            (17 * cos(10 * t)) / 2. + (22 * cos(11 * t)) / 3. + (15 * cos(12 * t)) / 4. - 
            (5 * cos(13 * t)) / 2. + (19 * cos(14 * t)) / 6. + (7 * cos(15 * t)) / 4. + 
            (31 * cos(16 * t)) / 4. - cos(17 * t) + (11 * cos(18 * t)) / 10. - (2 * cos(19 * t)) / 3. + 
            (13 * cos(20 * t)) / 3. - (5 * cos(21 * t)) / 4. + (2 * cos(22 * t)) / 3. + 
            cos(23 * t) / 4. + (5 * cos(24 * t)) / 6. + (3 * cos(26 * t)) / 4. - cos(27 * t) / 2. - 
            cos(28 * t) / 10. - cos(29 * t) / 3. - cos(30 * t) / 19. - 
            (637 * sin(t)) / 2. - (188 * sin(2 * t)) / 5. - (11 * sin(3 * t)) / 7. - (12 * sin(4 * t)) / 5. + 
            (11 * sin(5 * t)) / 3. - (37 * sin(6 * t)) / 4. + (8 * sin(7 * t)) / 3. + 
            (65 * sin(8 * t)) / 6. - (32 * sin(9 * t)) / 5. - (41 * sin(10 * t)) / 4. - 
            (38 * sin(11 * t)) / 3. - (47 * sin(12 * t)) / 8. + (5 * sin(13 * t)) / 4. - 
            (41 * sin(14 * t)) / 7. - (7 * sin(15 * t)) / 3. - (13 * sin(16 * t)) / 7. + 
            (17 * sin(17 * t)) / 4. - (9 * sin(18 * t)) / 4. + (8 * sin(19 * t)) / 9. + 
            (3 * sin(20 * t)) / 5. - (2 * sin(21 * t)) / 5. + (4 * sin(22 * t)) / 3. + sin(23 * t) / 3. + 
            (3 * sin(24 * t)) / 5. - (3 * sin(25 * t)) / 5. + (6 * sin(26 * t)) / 5. - sin(27 * t) / 5. + 
            (10 * sin(28 * t)) / 9. + sin(29 * t) / 3. - (3 * sin(30 * t)) / 4.) / 500.0;
}

double dfx_dt_numeric(double t)
{
    const double h = 0.000001;
    return (fx(t + h) - fx(t - h)) / (2.0 * h);
}

double dfy_dt_numeric(double t)
{
    const double h = 0.000001;
    return (fy(t + h) - fy(t - h)) / (2.0 * h);
}

double integrando_area_gato(double t)
{
    double dx = dfx_dt_numeric(t);
    double dy = dfy_dt_numeric(t);
    
    return 0.5 * (fx(t) * dy - fy(t) * dx);
}