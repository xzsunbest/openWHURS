run('/Applications/MATLAB_R2015a.app/toolbox/vlfeat-0.9.20/toolbox/vl_setup.m');
files = dir('Database/*.jpg');
for n = 1:numel(files)
    file = files(n);
    name = file.name;
    im_board_orig_color = im2double(imread(['Database/' name]));
    piece_infos = chess_state_detection(im_board_orig_color);
    save(['Results/piece_infos_' name(1:numel(name)-4) '.mat'],'piece_infos')
end