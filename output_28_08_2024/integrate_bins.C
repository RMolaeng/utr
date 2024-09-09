void integrate_bins() {
  
    // Define the integration limits
    double lowerLimit = 1.3995;
    double upperLimit = 1.4005;
    
    // Find the bin numbers corresponding to the limits
    int bin1 = he_s2_cal->GetXaxis()->FindBin(lowerLimit);
    int bin2 = he_s2_cal->GetXaxis()->FindBin(upperLimit);
    
    // Integrate the histogram over the specified bins
    double result = he_s2_cal->Integral(bin1, bin2);
    
    // Print the result
    std::cout << "Integral result: " << result << std::endl;
}