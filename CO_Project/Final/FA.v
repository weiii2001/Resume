`timescale 1ns/1ns
module FA( sum, cout, a, b, c );

    input a, b, c;
    output cout, sum;
    
    wire   e1, e2, e3;

    xor (e1, a, b);
    and (e2, a, b);
    and (e3, e1, c);
    or	(cout,e2, e3);
    xor  (sum, e1, c);

endmodule