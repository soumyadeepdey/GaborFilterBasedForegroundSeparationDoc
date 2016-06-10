clear all;
%Read the original RGB input image
          image=imread('3class_input2.png');
          [rows cols depth]=size(image );
      %convert it to gray scale
          image_gray=rgb2gray(image);
      %resize the image to 160x160 pixels
         % image_resize=imresize(image_gray, [160 160]);
      %apply im2double
          %image_resize=im2double(image_resize);
          image_resize=im2double(image_gray);
      %show the image
          figure(1);
          imshow(image_resize);
          tgaborArray = gaborFilterBank(5,8,rows,cols);  % Generates the Gabor filter bank
          %featureVector = gaborFeatures(image_resize,gaborArray,4,4);   % Extracts Gabor feature vector, 'featureVector', from the image, 'img'.itle('Input Image');
          
%           %Gabor filter size 7x7 and orientation 90 degree
%       %declare the variables
%           gamma=0.25; %aspect ratio
%           psi=0; %phase
%           theta=60; %orientation
%           bw=2.8; %bandwidth or effective width
%           lambda=3.5; % wavelength
%           pi=60;
%           for x=1:rows
%               for y=1:cols
%                  x_theta=image_resize(x,y)*cos(theta)+image_resize(x,y)*sin(theta);
%                  y_theta=-image_resize(x,y)*sin(theta)+image_resize(x,y)*cos(theta);
%           
%                  gb(x,y)= exp(-(x_theta.^2/2*bw^2+ gamma^2*y_theta.^2/2*bw^2))*cos(2*pi/lambda*x_theta+psi);
%           
%               end
%           end
%           figure(2);
%           imshow(gb);
%           title('filtered image');
%           
          clear all;