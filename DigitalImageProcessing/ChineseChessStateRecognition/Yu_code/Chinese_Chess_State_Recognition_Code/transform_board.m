function im_result = transform_board(im_color, im_board_golden)
    [height, width, ~] = size(im_color);
    im_board_orig_color = padarray(im_color, [max(height,width)-height,max(height,width)-width, 0]);
    im_board_orig = rgb2gray(im_board_orig_color);
    board_mask = generate_board_mask(im_board_orig);
    [T, corners, corners_golden] = find_homography_transformation(board_mask);
    im_result = align_and_crop(im_board_orig_color, im_board_golden, T, corners, corners_golden);
end

function board_mask = generate_board_mask(im_board)
    [height, width] = size(im_board);
    thresh = graythresh(im_board);
    im_board_only_bw = im2bw(im_board, thresh);
    figure;
    subplot(2,5,1);
    imshow(im_board);
    subplot(2,5,2);
    imshow(im_board_only_bw);
    im_board_only_bw = imfill(im_board_only_bw, 'holes');
    subplot(2,5,3);
    imshow(im_board_only_bw);
    se = strel('disk', fix(height/500), 4);
    im_board_only_bw = imerode(im_board_only_bw, se);
    subplot(2,5,4);
    imshow(im_board_only_bw);
    STATS=regionprops(im_board_only_bw,'Area','Centroid','BoundingBox');
    area=cat(1,STATS.Area);
    im_board_only_bw = bwareaopen(im_board_only_bw,fix(max(area(:))*0.5));
    subplot(2,5,5);
    imshow(im_board_only_bw);
    se = strel('disk', fix(height/500), 4);
    im_board_only_bw = imdilate(im_board_only_bw, se);
    subplot(2,5,6);
    imshow(im_board_only_bw);
    se = strel('disk', fix(height/50), 4);
    im_board_only_bw = imdilate(im_board_only_bw, se);
    subplot(2,5,7);
    imshow(im_board_only_bw);
    im_board_only_bw = imfill(im_board_only_bw, 'holes');
    subplot(2,5,8);
    imshow(im_board_only_bw);
    se = strel('disk', fix(height/50), 4);
    im_board_only_bw = imerode(im_board_only_bw, se);
    subplot(2,5,9);
    imshow(im_board_only_bw);
    STATS=regionprops(im_board_only_bw,'Area','Centroid','BoundingBox');
    area=cat(1,STATS.Area);
    im_board_only_bw = bwareaopen(im_board_only_bw,fix(max(area(:))*0.2));
    subplot(2,5,10);
    imshow(im_board_only_bw.*im_board);
    board_mask = im_board_only_bw;
end

function [T, corners, corners_golden] = find_homography_transformation(board_mask)
    corners = find_corners(board_mask);
    corners_golden = load('golden_corners.mat');
    corners_golden = corners_golden.corners;
    v=homography_solve(corners',corners_golden');
    T=projective2d(v');
    
end

function im_result = align_and_crop(im_board_orig_color, im_board_golden, T, corners, corners_golden)
    im_board_orig_h = imwarp(im_board_orig_color,T);
    figure;
    imshow(im_board_orig_h);
    im_board_orig = rgb2gray(im_board_orig_color);
    k1_im = double(zeros(size(im_board_orig)));
    k1_im(fix(corners(1,2)),fix(corners(1,1))) = 1;
    k1_im_h = imwarp(k1_im,T);
    [~, idx] = max(k1_im_h(:));
    [k1_y,k1_x] = ind2sub(size(k1_im_h),idx);
    im_board_orig_h_t = imtranslate(im_board_orig_h,[corners_golden(1,1)-k1_x, corners_golden(1,2)-k1_y]);
    [height_golden, width_golden] = size(im_board_golden);
    [height_h, width_h, ~] = size(im_board_orig_h_t);
    diff_height = 0;
    diff_width = 0;
    if height_h < height_golden
        diff_height = height_golden-height_h;
    end
    if width_h < width_golden
        diff_width = width_golden-width_h;
    end
    im_board_orig_h_t = padarray(im_board_orig_h_t, [diff_height diff_width], 'post');
    im_board_orig_h_t = im_board_orig_h_t(1:height_golden,1:width_golden,:);
    im_result = im_board_orig_h_t;
end