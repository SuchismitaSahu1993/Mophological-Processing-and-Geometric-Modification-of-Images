% clear
f = fopen('rice.raw','r');
data = fread(f,690*500*3,'uint8=>uint8');
j=1;

for i=1:3:(690*500*3)-2
    R(j)=data(i);
    j=j+1;
end

k=1;

for i=2:3:(690*500*3)-1
    G(k)=data(i);
    k=k+1;
end

l=1;

for i=3:3:(690*500*3)
    B(l)=data(i);
    l=l+1;
end

rice_r = reshape(R,690,500)';
rice_g = reshape(G,690,500)';
rice_b = reshape(B,690,500)';

original(:,:,1)=rice_r(:,:);
original(:,:,2)=rice_g(:,:);
original(:,:,3)=rice_b(:,:);
figure

imshow(original)
grayscale = rgb2gray(original);

BW = edge(grayscale,'canny', 0.1);
figure;
imshow(BW)

% Extract target regions
se90 = strel('line', 2, 90);
se0 = strel('line', 2, 0);
BW2 = imdilate(BW,[se90 se0]);
figure;
imshow(BW2)
BWfill = imfill(BW2,'holes');
figure;
imshow(BWfill)
seD = strel('diamond',4);
BWfinal = imerode(BWfill,seD);
figure;
imshow(BWfinal)



% Measuring the statistics
stats = regionprops(BWfinal,{'Area','Centroid'});
stats = struct2table(stats);
size(stats)

% Show the result
figure;
imshow(original)
hold on
for kk = 1:height(stats)
  text(stats.Centroid(kk,1)+10, stats.Centroid(kk,2),...
      num2str(stats.Area(kk)))
end

binary_image = imbinarize(grayscale);
figure;
imshow(binary_image)
bw = bwareaopen(binary_image,50);
figure;
imshow(bw)
cc = bwconncomp(bw,4);
cc.NumObjects

