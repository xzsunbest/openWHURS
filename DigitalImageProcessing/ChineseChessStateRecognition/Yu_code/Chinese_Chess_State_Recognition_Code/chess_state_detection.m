function piece_infos = chess_state_detection(im_board_orig_color)
    if nargin == 0
        run('/Applications/MATLAB_R2015a.app/toolbox/vlfeat-0.9.20/toolbox/vl_setup.m');
        im_board_orig_color = imresize(im2double(imread('Database/view_1.jpg')),1);    
    end
    im_board_golden_color = im2double(imread('board_empty_golden.jpg'));
    im_board_golden = rgb2gray(im_board_golden_color);
    figure;
    imshow(im_board_orig_color);
    im_board_top_down = transform_board(im_board_orig_color, im_board_golden);
    figure;
    imshow(0.5*(im_board_top_down+im_board_golden_color));
    piece_infos = char_recognition(im_board_top_down);
    piece_infos = color_recognition(piece_infos);
%     piece_infos = [];
end