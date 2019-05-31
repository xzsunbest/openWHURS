%%find_corners function from Spring 2012~2013 project Pool Table Edge,
%%Pocket and Ball Position Estimation.
function corners = find_corners(picture_edges)
index = 1;
picture_label=bwlabel(picture_edges,4);
STATS=regionprops(picture_label,'Area','Centroid','BoundingBox');
area=cat(1,STATS.Area);
centroid=cat(1,STATS.Centroid);
boundingbox=cat(1,STATS.BoundingBox);
cent=centroid(1,:);
bound=boundingbox(1,:);
vertex(1,1)=cent(1)-bound(3)/2;
vertex(1,2)=cent(2)-bound(4)/2;
vertex(2,1)=cent(1)+bound(3)/2;
vertex(2,2)=cent(2)-bound(4)/2;
vertex(3,1)=cent(1)-bound(3)/2;
vertex(3,2)=cent(2)+bound(4)/2;
vertex(4,1)=cent(1)+bound(3)/2;
vertex(4,2)=cent(2)+bound(4)/2;
start=round((vertex(3,2)-vertex(1,2))./4+vertex(1,2));
stop=round((vertex(3,2)-vertex(1,2)).*3/4+vertex(1,2));
picture_zero=picture_edges*0;
for y=start:stop
    x=1;
    % looks for binarized white edges of table
    while (picture_edges(y,x)==0)
        x=x+1;
    end
    % saves edges in arrays
    picture_zero(y,x)=1;
end
% Hough transform
[H,theta,rho]=hough(picture_zero,'Theta',-90:0.1:89.999);
peaks=houghpeaks(H,1);
t=theta(peaks(1,2))*pi/180;
r=rho(peaks(1,1));
if tan(t)==0
    slope=99999;
else
    slope=-1./tan(t);
end
if sin(t)==0
    intercept=-r(index)*slope;
else
    intercept=r./sin(t);
end

slope_left=slope;
intercept_left=intercept;


start=round((vertex(4,2)-vertex(2,2))./4+vertex(2,2));
stop=round((vertex(4,2)-vertex(2,2)).*3/4+vertex(2,2));
picture_zero=picture_edges*0;
for y=start:stop
    x=size(picture_edges,2);
    % looks for binarized white edges of table
    while (picture_edges(y,x)==0)
        x=x-1;
    end
    % saves edges in arrays
    picture_zero(y,x)=1;
end
% Hough transform
[H,theta,rho]=hough(picture_zero,'Theta',-90:0.1:89.999);
peaks=houghpeaks(H,1);
t=theta(peaks(1,2))*pi/180;
r=rho(peaks(1,1));
if tan(t)==0
    slope=99999;
else
    slope=-1./tan(t);
end
if sin(t)==0
    intercept=-r(index)*slope;
else
    intercept=r./sin(t);
end

slope_right=slope;
intercept_right=intercept;


start=round((vertex(2,1)-vertex(1,1))./4+vertex(1,1));
stop=round((vertex(2,1)-vertex(1,1)).*3/4+vertex(1,1));
picture_zero=picture_edges*0;
for x=start:stop
    y=1;
    % looks for binarized white edges of table
    while (picture_edges(y,x)==0)
        y=y+1;
    end
    % saves edges in arrays
    picture_zero(y,x)=1;
end
% Hough transform
[H,theta,rho]=hough(picture_zero,'Theta',-90:0.1:89.999);
peaks=houghpeaks(H,1);
t=theta(peaks(1,2))*pi/180;
r=rho(peaks(1,1));
if tan(t)==0
    slope=99999;
else
    slope=-1./tan(t);
end
if sin(t)==0
    intercept=-r(index)*slope;
else
    intercept=r./sin(t);
end

slope_top=slope;
intercept_top=intercept;


start=round((vertex(4,1)-vertex(3,1))./4+vertex(3,1));
stop=round((vertex(4,1)-vertex(3,1)).*3/4+vertex(3,1));
picture_zero=picture_edges*0;
for x=start:stop
    y=size(picture_edges,1);
    % looks for binarized white edges of table
    while (picture_edges(y,x)==0)
        y=y-1;
    end
    % saves edges in arrays
    picture_zero(y,x)=1;
end
% Hough transform
[H,theta,rho]=hough(picture_zero,'Theta',-90:0.1:89.999);
peaks=houghpeaks(H,1);
t=theta(peaks(1,2))*pi/180;
r=rho(peaks(1,1));
if tan(t)==0
    slope=99999;
else
    slope=-1./tan(t);
end
if sin(t)==0
    intercept=-r(index)*slope;
else
    intercept=r./sin(t);
end



slope_bottom=slope;
intercept_bottom=intercept;


corner_upper_left(1)=(intercept_left-intercept_top)./(slope_top-slope_left);
corner_upper_left(2)=slope_top.*corner_upper_left(1)+intercept_top;

corner_upper_right(1)=(intercept_right-intercept_top)./(slope_top-slope_right);
corner_upper_right(2)=slope_top.*corner_upper_right(1)+intercept_top;

corner_lower_left(1)=(intercept_left-intercept_bottom)./(slope_bottom-slope_left);
corner_lower_left(2)=slope_bottom.*corner_lower_left(1)+intercept_bottom;

corner_lower_right(1)=(intercept_right-intercept_bottom)./(slope_bottom-slope_right);
corner_lower_right(2)=slope_bottom.*corner_lower_right(1)+intercept_bottom;
corners = [corner_upper_left(1),corner_upper_left(2);...
    corner_upper_right(1),corner_upper_right(2);...
    corner_lower_left(1),corner_lower_left(2);
    corner_lower_right(1),corner_lower_right(2)];
end
