function [ ] = plotBlob( img, frames )

figure, clf;
imshow(img); hold on;
thetaVec = linspace(0,2*pi,3600);
    for n = 1:size(frames,1)
        x0 = frames(n,1);
        y0 = frames(n,2);
        r = 1.8 * frames(n,3);
        xVec = x0 + r*sin(thetaVec);
        yVec = y0 + r*cos(thetaVec);
        plot(xVec-1, yVec-1, 'k-', 'LineWidth', 1, 'LineSmoothing','on');
        plot(xVec, yVec, 'y-', 'LineWidth', 1, 'LineSmoothing','on');
    end % n

end



