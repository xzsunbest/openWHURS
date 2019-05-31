function piece_infos = char_recognition(im_board_top_down)
    r = load('golden_row_locs.mat');
    r = r.r;
    c = load('golden_col_locs.mat');
    c = c.c;
    delta = 100;
    [height, width, ~] = size(im_board_top_down);
    figure;

    refs_black = load('chess_pieces_ref_black.mat');
    refs_black = refs_black.chess_piece_refs;
    refs_red = load('chess_pieces_ref_red.mat');
    refs_red = refs_red.chess_piece_refs;
    start_i = 1;
    start_j = 1;
    end_i = numel(r);
    end_j = numel(c);

    for j = start_j:end_j
        for i = start_i:end_i
            h = r(i);
            w = c(j);
            im_piece = im_board_top_down(h-delta:h+delta,w-delta:w+delta,:);
            subplot(end_i,end_j,(i-1)*end_j+j);
            imshow(im_piece);
            [centers, radii, metric] = imfindcircles(im_piece,[45 55],...
                'Sensitivity', 0.95);
            if numel(radii)
                im_center = [delta+1 delta+1];
                im_centers = repmat(im_center, [numel(radii) 1]);
                dists = sqrt(sum((im_centers-centers).^2,2));
                [val, idx] = min(dists);
                centersStrong = centers(idx,:);
                radiiStrong = radii(idx);
                metricStrong = metric(idx); 
                dist = dists(idx);
                if dist < radiiStrong*1.5
                    viscircles(centersStrong, radiiStrong,'EdgeColor','b');
                    im_piece_gray = rgb2gray(im_piece);
                    [height_piece,width_piece] = size(im_piece_gray);
                    im_mask_gray = zeros(height_piece, width_piece);
                    for k = 1:height_piece
                        for l = 1:width_piece
                            pt = [l, k];
                            dist = sqrt(sum((pt-centersStrong).^2));
                            if dist <= radiiStrong
                                im_mask_gray(k,l) = 1;
                            end
                        end
                    end
                    im_mask = repmat(im_mask_gray, [1 1 3]);
                    im_piece_masked = im_piece.*im_mask;
                    match_max = 0;
                    for refs_black_key_cell = refs_black.keys
                        refs_black_key = refs_black_key_cell{1};
                        im_ref = refs_black(refs_black_key);
                        sift_result = sift_match(im_ref, rgb2gray(im_piece_masked));
                        if sift_result.ransac > match_max
                            match_max = sift_result.ransac;
                            match_key = refs_black_key;
                            match_color = 'black';
                        end
                    end
                    for refs_red_key_cell = refs_red.keys
                        refs_red_key = refs_red_key_cell{1};
                        im_ref = refs_red(refs_red_key);
                        sift_result = sift_match(im_ref, rgb2gray(im_piece_masked));
                        if sift_result.ransac > match_max
                            match_max = sift_result.ransac;
                            match_key = refs_red_key;
                            match_color = 'red';
                        end
                    end
                    if match_max > 0
                        piece_infos(i, j).name = match_key;
                        piece_infos(i, j).color = match_color;
                        piece_infos(i, j).im = im_piece_masked;
                    else
                        piece_infos(i, j).name = 'none';
                        piece_infos(i, j).color = 'none';
                        piece_infos(i, j).im = [];
                    end
%                     piece_infos(i, j)
                end
            end
        end
    end
end