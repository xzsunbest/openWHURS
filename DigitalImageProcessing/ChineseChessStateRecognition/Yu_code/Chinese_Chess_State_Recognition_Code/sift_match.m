function matchResult = sift_match(im1, im2, varargin)
% SIFT_MATCH Match two images using SIFT and RANSAC
%
%   SIFT_MATCH demonstrates matching two images based on SIFT
%   features and RANSAC.
%
%   SIFT_MATCH by itself runs the algorithm on two standard test
%   images. Use SIFT_MATCH(IM1,IM2) to compute the matches of two
%   custom images IM1 and IM2.
%
%   SIFT_MATCH can also run on two pre-computed sets of features.
%   Use SIFT_MATCH(IM1, IM2, FEAT1, FEAT2), where FEAT1.f and FEAT1.d
%   represent the SIFT frames and descriptors of the first image.
%
%   SIFT_MATCH returns MATCHRESULT, where 
%   -- MATCHRESULT.RATIO_TEST reports the number of correspondences 
%   after the distance ratio test
%   -- MATCHRESULT.RANSAC reports the number of correspondences
%   after the distance ratio test + RANSAC with a homography
%   -- MATCHRESULT.MODEL contains the best homography found
%   -- MATCHRESULTS.MATCHES contains the indices of the matching
%   features in the two images

% AUTORIGHTS

if nargin == 0
  im1 = imread(fullfile(vl_root, 'data', 'river1.jpg')) ;
  im2 = imread(fullfile(vl_root, 'data', 'river2.jpg')) ;
end

% make single
im1 = im2single(im1) ;
im2 = im2single(im2) ;

% make grayscale
if size(im1,3) > 1, im1g = rgb2gray(im1) ; else im1g = im1 ; end
if size(im2,3) > 1, im2g = rgb2gray(im2) ; else im2g = im2 ; end

% --------------------------------------------------------------------
%                                                         SIFT matches
% --------------------------------------------------------------------

if length(varargin) >= 2
    set1 = varargin{1};
    set2 = varargin{2};
    f1 = set1.f;
    d1 = set1.d;
    f2 = set2.f;
    d2 = set2.d;
else
    [f1,d1] = vl_sift(im1g) ;
    [f2,d2] = vl_sift(im2g) ;
end

% Perform distance ratio test
[matches, scores] = vl_ubcmatch(d1,d2) ;

% Remove many-to-one matches
[uniqueRow2, IA, IC] = unique(matches(2,:));
uniqueRow1 = matches(1,IA);
matches = [uniqueRow1; uniqueRow2];
numMatches = size(matches,2) ;

X1 = f1(1:2,matches(1,:)) ; X1(3,:) = 1 ;
X2 = f2(1:2,matches(2,:)) ; X2(3,:) = 1 ;

% --------------------------------------------------------------------
%                                         RANSAC with homography model
% --------------------------------------------------------------------

clear H score ok ;
radius = 6;
for t = 1:100
  % estimate homography
  subset = vl_colsubset(1:numMatches, 4) ;
  A = [] ;
  for i = subset
    A = cat(1, A, kron(X1(:,i)', vl_hat(X2(:,i)))) ;
  end
  [U,S,V] = svd(A) ;
  if numel(V) == 0
      V = zeros(9,9);
  end
  H{t} = reshape(V(:,9),3,3) ;

  % score homography
  X2_ = H{t} * X1 ;
  du = X2_(1,:)./X2_(3,:) - X2(1,:)./X2(3,:) ;
  dv = X2_(2,:)./X2_(3,:) - X2(2,:)./X2(3,:) ;
  ok{t} = (du.*du + dv.*dv) < radius*radius ;
  score(t) = sum(ok{t}) ;
end

[score, best] = max(score) ;
H = H{best} ;
ok = ok{best} ;

% --------------------------------------------------------------------
%                                                  Optional refinement
% --------------------------------------------------------------------

function err = residual(H)
 u = H(1) * X1(1,ok) + H(4) * X1(2,ok) + H(7) ;
 v = H(2) * X1(1,ok) + H(5) * X1(2,ok) + H(8) ;
 d = H(3) * X1(1,ok) + H(6) * X1(2,ok) + 1 ;
 du = X2(1,ok) - u ./ d ;
 dv = X2(2,ok) - v ./ d ;
 err = sum(du.*du + dv.*dv) ;
end

if exist('fminsearch') == 2
  H = H / H(3,3) ;
  opts = optimset('Display', 'none', 'TolFun', 1e-8, 'TolX', 1e-8) ;
  H(1:8) = fminsearch(@residual, H(1:8)', opts) ;
else
%   warning('Refinement disabled as fminsearch was not found.') ;
end

% --------------------------------------------------------------------
%                                                         Show matches
% --------------------------------------------------------------------

warning off;
dh1 = max(size(im2,1)-size(im1,1),0) ;
dh2 = max(size(im1,1)-size(im2,1),0) ;

% figure(1) ; 
% subplot(2,1,1) ;

% subplot(2,2,1);
% imshow([padarray(im1,dh1,'post') padarray(im2,dh2,'post')]) ;
% o = size(im1,2) ;
% figure(2) ; 
% figure;
% subplot(2,2,2);
% imshow([padarray(im1,dh1,'post') padarray(im2,dh2,'post')]) ;
% hold on;
% h1 = vl_plotframe(f1) ;
% h2 = vl_plotframe(f1) ;
% set(h1,'color','k','linewidth',3) ;
% set(h2,'color','y','linewidth',2) ;
% 
% fTemp = f2;
% fTemp(1, :) = fTemp(1, :) + o;
% h1 = vl_plotframe(fTemp) ;
% h2 = vl_plotframe(fTemp) ;
% set(h1,'color','k','linewidth',3) ;
% set(h2,'color','y','linewidth',2) ;

% figure(3) ; clf ;
% figure;
% subplot(2,2,3);
% imshow([padarray(im1,dh1,'post') padarray(im2,dh2,'post')]) ;
% hold on;
% o = size(im1,2) ;
% plot([f1(1,matches(1,:));f2(1,matches(2,:))+o], ...
%      [f1(2,matches(1,:));f2(2,matches(2,:))], ['k' '-'],  'LineWidth', 2) ;
% plot([f1(1,matches(1,:)) + 1;f2(1,matches(2,:))+o+1], ...
%      [f1(2,matches(1,:)) + 1;f2(2,matches(2,:))+1], ['y' '-'],  'LineWidth', 2) ;
% plot(f1(1,matches(1,:)), f1(2,matches(1,:)), 'ko', 'MarkerSize', 5, 'MarkerFaceColor', 'k'); 
% plot(f1(1,matches(1,:))+1, f1(2,matches(1,:))+1, 'yo', 'MarkerSize', 5, 'MarkerFaceColor', 'y'); 
% plot(f2(1,matches(2,:))+o, f2(2,matches(2,:)), 'ko', 'MarkerSize', 5, 'MarkerFaceColor', 'k');
% plot(f2(1,matches(2,:))+o+1, f2(2,matches(2,:))+1, 'ko', 'MarkerSize', 5, 'MarkerFaceColor', 'y');

% figure(4) ; clf ;
% % figure;
% subplot(2,2,4);
% imshow([padarray(im1,dh1,'post') padarray(im2,dh2,'post')]) ;
% o = size(im1,2) ;
% hold on;
% plot([f1(1,matches(1,ok));f2(1,matches(2,ok))+o], ...
%      [f1(2,matches(1,ok));f2(2,matches(2,ok))], ['k' '-'],  'LineWidth', 2) ;
% plot([f1(1,matches(1,ok))+1;f2(1,matches(2,ok))+o+1], ...
%      [f1(2,matches(1,ok))+1;f2(2,matches(2,ok))+1], ['y' '-'],  'LineWidth', 2) ;  
% plot(f1(1,matches(1,ok)), f1(2,matches(1,ok)), 'ko', 'MarkerSize', 5, 'MarkerFaceColor', 'k'); 
% plot(f1(1,matches(1,ok))+1, f1(2,matches(1,ok))+1, 'yo', 'MarkerSize', 5, 'MarkerFaceColor', 'y'); 
% plot(f2(1,matches(2,ok))+o, f2(2,matches(2,ok)), 'ko', 'MarkerSize', 5, 'MarkerFaceColor', 'k');
% plot(f2(1,matches(2,ok))+o+1, f2(2,matches(2,ok))+1, 'ko', 'MarkerSize', 5, 'MarkerFaceColor', 'y');
% 
% drawnow ;

matchResult.ratio_test = size(matches,2);
matchResult.ransac = sum(ok);
matchResult.model = H;
matchResult.matches = matches(:,ok);
matchResult.f1 = f1;
matchResult.f2 = f2;
matchResult.d1 = d1;
matchResult.d2 = d2;

warning on;

end