module top( x0 , x1 , x2 , x3 , y0 , y1 );
  input x0 , x1 , x2 , x3 ;
  output y0 , y1 ;
  wire n5 , n6 ;
  assign n5 = x2 & x3 ;
  assign n6 = x1 & n5 ;
  assign y0 = n5 ;
  assign y1 = ~n6 ;
endmodule
